
#include "gmock/gmock.h"

#include <iostream>
#include <string>
#include <memory>
#include <algorithm>


using namespace testing;

namespace runtime_polymorphism
{



	namespace _1
	{
		using object_t = int;
		using document_t = std::vector<object_t>;

		void draw(const object_t& x, std::ostream& out, size_t position)
		{
			out << std::string(position, ' ') << x << std::endl;
		}

		void draw(const document_t& x, std::ostream& out, size_t position)
		{
			out << "<document>" << std::endl;
			for (const auto& e : x)
			{
				draw(e, out, position + 2);
			}
			out << "<document/>" << std::endl;
		}


		TEST(Runtime_Polymorphism, print_int)
		{
			document_t doc{ 0,1,2,3,4 };
			draw(doc, std::cout, 2);
		}
	}


	namespace _2
	{
		class object_t
		{
		public:
			virtual ~object_t() = default;
			virtual void draw(std::ostream& out, size_t position) = 0;
		};

		using document_t = std::vector<std::shared_ptr<object_t>>;



		class my_class_t :public object_t
		{
			void draw(std::ostream& out, size_t position) override
			{
				out << std::string(position, ' ') << "my_class" << std::endl;
			}
		};


		void draw(const document_t& x, std::ostream& out, size_t position)
		{
			out << "<document>" << std::endl;
			for (const auto& e : x)
			{
				e->draw(out, position + 2);
			}
			out << "<document/>" << std::endl;
		}


		TEST(Runtime_Polymorphism, print_drable)
		{
			document_t doc;

			doc.push_back(std::make_shared<my_class_t>());

			draw(doc, std::cout, 2);
		}
	}


	namespace _3
	{
		class object_t
		{
		public:
			object_t(const int& x) :self_(x) {}

			friend void draw(const object_t& x, std::ostream& out, size_t position)
			{
				out << std::string(position, ' ') << x.self_ << std::endl;
			}
		private:
			int self_;
		};

		using document_t = std::vector<object_t>;

		void draw(const document_t& x, std::ostream& out, size_t position)
		{
			out << "<document>" << std::endl;
			for (const auto& e : x)
			{
				draw(e, out, position + 2);
			}
			out << "<document/>" << std::endl;
		}


		TEST(Runtime_Polymorphism, print_wrapper_int)
		{
			document_t doc;

			doc.push_back(0);
			doc.push_back(1);
			doc.push_back(2);

			draw(doc, std::cout, 2);
		}
	}


	namespace _4
	{
		class object_t
		{
		public:
			object_t( int x) :self_(std::make_unique<int_model_t>(std::move(x))) {
				std::cout << "ctor" << std::endl;
			}
			object_t(const object_t& x) :self_(std::make_unique<int_model_t>(*x.self_)) {
				std::cout << "copy" << std::endl;
			}

//			object_t(object_t&& x) :self_(std::move(x.self_)) { }
			object_t(object_t&& x) noexcept = default;

			object_t& operator=(const object_t& x) noexcept{
				return *this = object_t(x);
			}

			object_t& operator=(object_t&& x) noexcept = default;

			friend void draw(const object_t& x, std::ostream& out, size_t position)
			{
				x.self_->draw_(out, position);
			}

		private:
			struct int_model_t
			{
				int_model_t(int x) :data_(std::move(x)) {}

				void draw_(std::ostream& out, size_t position) const { 
					out << std::string(position, ' ') << data_ << std::endl;
				}

				int data_;

			};
			std::unique_ptr<int_model_t> self_;
		};

		using document_t = std::vector<object_t>;

		void draw(const document_t& x, std::ostream& out, size_t position)
		{
			out << "<document>" << std::endl;
			for (const auto& e : x)
			{
				draw(e, out, position + 2);
			}
			out << "<document/>" << std::endl;
		}


		TEST(Runtime_Polymorphism, print_int_model)
		{
			document_t doc;

			doc.emplace_back(0);
			doc.emplace_back(1);
			doc.emplace_back(2);

			draw(doc, std::cout, 2);
		}


		object_t func()
		{
			object_t result = 5;
			return result;
		}

		TEST(Runtime_Polymorphism, function_return_optimaztion)
		{
			//in release mode, there is only on ctor call
			auto r = func();
			r = 3;
		}

		TEST(Runtime_Polymorphism, function_return_optimaztion_assign_first)
		{
			object_t x = 0;
			x = func();
		}

		TEST(Runtime_Polymorphism, print_int_model_reverse)
		{
			document_t doc;

			doc.emplace_back(0);
			doc.emplace_back(1);
			doc.emplace_back(2);

			std::reverse(doc.begin(), doc.end());
			draw(doc, std::cout, 2);
		}

		struct some_t
		{
			object_t o;
		};

		some_t func1() { return { 5 }; }

		TEST(Runtime_Polymorphism, will_not_move_when_wrapped_by_a_class_that_doesnt_has_move_ctor)
		{
			some_t x{ 0 };

			x = func1();
		}


	}


	namespace _5
	{


		template<typename T>
		void draw(const T& x, std::ostream& out, size_t position)
		{
			out << std::string(position, ' ') << x << std::endl;
		}

		template<typename T>
		void draw(const std::vector<T>& x, std::ostream& out, size_t position)
		{
			out << "<document>" << std::endl;
			for (const auto& e : x)
			{
				draw(e, out, position + 2);
			}
			out << "<document/>" << std::endl;
		}

		class object_t
		{
		public:
			template<typename T>
			object_t( T x) :self_(std::make_unique<model<T>>(std::move(x))) { }
			object_t(const object_t& x) :
				self_( x.self_->copy_() )
			{ }
			object_t(object_t&& x) noexcept = default;
			object_t& operator=(const object_t& x) noexcept{ return *this = object_t(x); }
			object_t& operator=(object_t&& x) noexcept = default;

			friend void draw(const object_t& x, std::ostream& out, size_t position)
			{
				x.self_->draw_(out, position);
			}

		private:
			struct concept_t
			{
				virtual ~concept_t() {};
				virtual void draw_(std::ostream& out, size_t position) const = 0;
				virtual std::unique_ptr<concept_t>copy_() const = 0;
			};

			template<typename T>
			struct model final :public concept_t
			{
				model(T x) :data_(std::move(x)) {}

				std::unique_ptr<concept_t> copy_() const override { return std::make_unique<model>(*this); }


				void draw_(std::ostream& out, size_t position) const override{ 
					draw(data_, out, position);
				}

				T data_;

			};

			std::unique_ptr<concept_t> self_;
		};


		using document_t = std::vector<object_t>;

		TEST(Runtime_Polymorphism, print_template_model)
		{
			document_t doc;

			doc.emplace_back(0);
			doc.emplace_back("abc");
			doc.emplace_back(doc);
			doc.emplace_back(2);

			draw(doc, std::cout, 2);
		}


	}

	namespace _6
	{


		template<typename T>
		void draw(const T& x, std::ostream& out, size_t position)
		{
			out << std::string(position, ' ') << x << std::endl;
		}

		template<typename T>
		void draw(const std::vector<T>& x, std::ostream& out, size_t position)
		{
			out << "<document>" << std::endl;
			for (const auto& e : x)
			{
				draw(e, out, position + 2);
			}
			out << "<document/>" << std::endl;
		}

		class object_t
		{
		public:
			template<typename T>
			object_t( T x) :self_(std::make_shared<model<T>>(std::move(x))) { }

			friend void draw(const object_t& x, std::ostream& out, size_t position)
			{
				x.self_->draw_(out, position);
			}

		private:
			struct concept_t
			{
				virtual ~concept_t() {};
				virtual void draw_(std::ostream& out, size_t position) const = 0;
				virtual std::unique_ptr<concept_t>copy_() const = 0;
			};

			template<typename T>
			struct model final :public concept_t
			{
				model(T x) :data_(std::move(x)) {}

				std::unique_ptr<concept_t> copy_() const override { return std::make_unique<model>(*this); }


				void draw_(std::ostream& out, size_t position) const override{ 
					draw(data_, out, position);
				}

				T data_;

			};

			std::shared_ptr<concept_t const> self_;
		};


		using document_t = std::vector<object_t>;

		using history_t = std::vector<document_t>;

		void commit(history_t& h)
		{
			if(!h.empty())
			{ 
				h.push_back(h.back());
			}
		}

		void undo(history_t& h)
		{
			if(!h.empty())
			{ 
				h.pop_back();
			}
		}

		document_t& current(history_t& h) {
			if(!h.empty())
				return h.back();
		}

		TEST(Runtime_Polymorphism, undo_redo)
		{

			history_t h(1);
			current(h).push_back(0);
			current(h).push_back(std::string("hello"));

			draw(current(h), std::cout, 0);

			std::cout << "--------------------------" << std::endl;
			commit(h);

			current(h)[0]=42.5;
			current(h)[1]=4;
			current(h).push_back(std::string("world"));
			draw(current(h), std::cout, 0);

			std::cout << "--------------------------" << std::endl;
			undo(h);
			draw(current(h), std::cout, 0);
		}


	}

}
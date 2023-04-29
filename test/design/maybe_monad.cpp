
#include "gmock/gmock.h"
#include <string>
#include <vector>
#include <type_traits>


using namespace testing;

namespace maybe_monad
{
	struct A
	{
		std::vector<std::string> f() 
		{
			return { "A" }; 
		}
	};

	struct B 
	{ 
		A* a = nullptr;

	};
	struct C 
	{ 
		B* b = nullptr;
	};




	template<typename T> 
	struct Maybe
	{
		Maybe(T* p) :p(p) {}

		template<typename F>
		auto with(F f)
		{
			using retT = std::remove_pointer_t<decltype(f(std::declval<T*>()))>;
			if (p)
			{
				return Maybe<retT>(f(p));
			}
			else
			{
				return Maybe<retT>(nullptr);
			}
		}

		template<typename F>
		std::vector<std::string> perform(F f)
		{
			if (p)
			{
				return f(p);
			}
			else
			{
				return {};
			}
		}

	private:
		T* p = nullptr;
	};

	TEST(Maybe_Monad_Test, test_nullptr_chain_break_with_nullptr)
	{
		C c;
		Maybe m{ &c };

		auto act = m.with([](auto x) {	return x->b; })
			.with([](auto x) { return x->a; })
			.perform([](auto x) { return x->f(); });

		std::vector<std::string> exp = {};
		EXPECT_THAT(act, Eq(exp));
	}

	TEST(Maybe_Monad_Test, test_pointer_chain_call_the_deeppest_pointer)
	{
		A a;
		B b{ &a };
		C c{ &b };
		Maybe m{ &c };

		auto act = m.with([](auto x) {	return x->b; })
			.with([](auto x) { return x->a; })
			.perform([](auto x) { return x->f(); });

		std::vector<std::string> exp = { "A" };
		EXPECT_THAT(act, Eq(exp));
	}


	namespace no_owning_ptr
	{

		template<typename T>
		class NoOwningPtr
		{
		public:
			NoOwningPtr(std::shared_ptr<T> obj) :m_data(obj) {};
			template<typename Fn>
			decltype(auto) invoke(Fn fn)
			{
				if (auto tmp = m_data.lock())
				{
					return fn(tmp);
				}
				else
				{
					using retT = decltype(fn(std::declval<std::shared_ptr<T>>()));
					return retT{};
				}
			}

		private:
			std::weak_ptr<T> m_data;
		};

		struct A 
		{
			std::string f() {
				return "A";
			}
		};

#define SAFE_INVOKE(f) invoke([](auto a) {return a->f(); })

		TEST(NoOwningPtr, invoke_empty_obj)
		{
			std::shared_ptr<A> ptr;
			NoOwningPtr w_ptr(ptr);

			std::string exp = "";

			std::string act = w_ptr.SAFE_INVOKE(f);
			EXPECT_THAT(act, Eq(exp));

		}

		TEST(NoOwningPtr, invoke_non_empty_obj)
		{
			std::shared_ptr<A> ptr = std::make_shared<A>();
			NoOwningPtr w_ptr(ptr);

			std::string exp = "A";

			std::string act = w_ptr.SAFE_INVOKE(f);

			EXPECT_THAT(act, Eq(exp));

		}

	}


}

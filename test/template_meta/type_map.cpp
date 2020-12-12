

#include "gmock/gmock.h"
#include <any>
#include <map>
#include <string>
#include <concepts>
#include <type_traits>


namespace type_map
{


	namespace _1
	{

		class Type_Map
		{
		public:

			template<typename T>
			void add(T obj)
			{
				std::string key = typeid(T).name();
				m_map[key] = std::any(obj);
			}

			template<typename T>
			T get()
			{
				std::string key = typeid(T).name();
				if (m_map.count(key))
				{
					return std::any_cast<T>(m_map[key]);
				}
				return T{};
			}

		private:
			std::map<std::string, std::any> m_map;
		};


		class A
		{

		};

		class B
		{

		};


		TEST(Type_Map_Test, add_get)
		{

			Type_Map m;
			m.add(A{});

			auto a = m.get<A>();
			auto aisA = typeid(a).name() == typeid(A).name();

			EXPECT_TRUE(aisA);
		}
	}


	////////////////////////////////////////////////////////////////////////////////

	namespace _2
	{

		class Base
		{
		public:
			virtual ~Base() {}

		};

		class A :public Base
		{
		public:
			void doA() {}

		};

		class B :public Base
		{
		public:
			void doB() {}
		};

		class kA { };
		class kB { };


		class Type_Map
		{
		public:

			template<typename T>
			struct id { using type = T; };

			template<typename T>
			struct Mapper;

			template<> struct Mapper<kA> :public id<A> {};
			template<> struct Mapper<kB> :public id<B> {};

			////////////////////////////////////////////


			template<typename T>
			auto& get_singleton()
			{
				static Mapper<T>::type t;
				return  t;
			}

			template<typename T>
			auto& get_singleton(T* ka)
			{
				static Mapper<T>::type t;
				return  t;
			}

		};



		TEST(Type_Map_Test, get_by_type)
		{

			Type_Map m;

			auto a = m.get_singleton<kA>();
			a.doA();

			auto b = m.get_singleton<kB>();
			b.doB();

			auto aisA = typeid(a).name() == typeid(A).name();

			EXPECT_TRUE(aisA);
		}

		TEST(Type_Map_Test, get_by_obj_by_key_type)
		{

			kA ka;
			kB kb;

			Type_Map m;

			auto a = m.get_singleton(&ka);
			a.doA();

			auto b = m.get_singleton(&kb);
			b.doB();

			auto aisA = typeid(a).name() == typeid(A).name();

			EXPECT_TRUE(aisA);
		}

	}

	namespace _3
	{

		//production class
		class Key_Base
		{
		public:
			virtual ~Key_Base() {}
			static int get_static_type() { return 0; }
			virtual int get_dynamic_type() { return 0; }
		};

		class Key_A :public Key_Base
		{
		public:
			static int get_static_type() { return 1; }
			int get_dynamic_type() override { return 1; }

		};

		class Key_B :public Key_Base
		{
		public:
			static int get_static_type() { return 2; }
			int get_dynamic_type() override { return 2; }
		};

		class Param_Base
		{
		public:
			virtual ~Param_Base() {}
			static int get_static_type() { return 0; }
			virtual int get_dynamic_type() { return 0; }
		};

		class Param_A :public Param_Base
		{
		public:
			static int get_static_type() { return 1; }
			int get_dynamic_type() override { return 1; }

		};

		class Param_B :public Param_Base
		{
		public:
			static int get_static_type() { return 2; }
			int get_dynamic_type() override { return 2; }
		};

		class Func_A
		{
		public:
			std::string apply(Key_A* key_a, Param_A* pa) { return "func_a(key_a,param_a)"; }

		};

		class Func_B
		{
		public:
			std::string apply(Key_B* key_b, Param_B* pb) { return "func_a(key_b,param_b)"; }
		};
		//production class


		//type_list

		template<typename ...P>
		struct type_list {};

		template<typename L>
		struct get_front_imp;

		template<typename H, typename ...T>
		struct get_front_imp<type_list<H, T...>>
		{
			using type = H;
		};

		template<typename L>
		using get_front = typename get_front_imp<L>::type;

		template<typename L>
		struct pop_front_imp;

		template<typename H, typename ...T>
		struct pop_front_imp<type_list<H, T...>>
		{
			using type = type_list<T...>;
		};

		template<typename L>
		using pop_front = typename pop_front_imp<L>::type;

		template<typename L>
		struct is_empty_imp :std::false_type {};

		template<>
		struct is_empty_imp<type_list<>> :std::true_type {};

		template<typename L>
		inline constexpr bool is_empty = is_empty_imp<L>::value;

		//type_list


			//struct reflection
		struct to_any
		{

			template<typename T>
			operator T* ();

			template<typename T>
			operator T& ();

		};

		template<typename T, typename U = void, typename... to_any_type>
		struct get_field_num
		{
			static constexpr int value = sizeof...(to_any_type) - 1;
		};

		template<typename T, typename... to_any_type>
		struct get_field_num < T, std::void_t< decltype(T{ to_any_type{}... }) > , to_any_type... >
		{
			static constexpr int value = get_field_num<T, void, to_any, to_any_type... >::value;
		};


		template<typename T>
		constexpr auto as_tuple()
		{
			constexpr int field_count = get_field_num<T>::value;

			if constexpr (field_count == 1)
			{
				auto [a] = T{};
				return std::make_tuple(a);
			}
			else if constexpr (field_count == 2)
			{
				auto [a, b] = T{};
				return std::make_tuple(a, b);
			}
			else if constexpr (field_count == 3)
			{
				auto [a, b, c] = T{};
				return std::make_tuple(a, b, c);
			}
		}

		template<typename tu>
		struct extract_tuple_to_type_list;

		template<typename... P>
		struct extract_tuple_to_type_list<std::tuple<P...>>
		{
			using type = type_list<std::remove_pointer_t<P>...>;
		};

		template<typename T>
		using extract_member_types = typename extract_tuple_to_type_list<decltype(as_tuple<T>())>::type;
		//struct reflection



				//map relations
		template <typename c0, typename c1, typename c2>
		struct Record
		{
			using type0 = c0;
			using type1 = c1;
			using type2 = c2;
		};

#define DEF_MEM(x,a,b) \
struct x##_Record : Record<x, a,b> {}; \
x##_Record* x##_var; \

		struct Type_Map_Records
		{
			DEF_MEM(Key_A, Param_A, Func_A);
			DEF_MEM(Key_B, Param_B, Func_B);

		};

		using all_records = extract_member_types<Type_Map_Records>;

		template<typename L, typename F, typename ...P>
		void for_each_type(P&& ... p)
		{
			if constexpr (!is_empty<L>)
			{
				using cur_t = get_front<L>;
				using poped_list = pop_front<L>;

				F::template apply<cur_t>(std::forward<P>(p)...);

				for_each_type<poped_list, F>(std::forward<P>(p)...);
			}

		};



		struct Apply_Func
		{
			template<typename cur_t>
			static void apply(Key_Base* key, Param_Base* p, std::vector<std::string>& result)
			{
				using kt = typename cur_t::type0;
				using t = typename cur_t::type1;
				using func = typename cur_t::type2;
				if (key->get_dynamic_type() == kt::get_static_type())
				{
					static func ins;
					auto r = ins.apply(dynamic_cast<kt*>(key), dynamic_cast<t*>(p));
					result.push_back(r);
				}
			}
		};



		//use key base but call sub type
		TEST(Type_Map_Test, get_by_obj_by_key_variable)
		{


			Key_A ka;
			Key_Base* key_base = &ka;

			Param_A pa;
			Param_Base* p_base = &pa;

			std::vector<std::string> act;
			for_each_type<all_records, Apply_Func>(key_base, p_base, act);

			std::vector<std::string> exp
			{
				"func_a(key_a,param_a)"
			};


		}



	}



}
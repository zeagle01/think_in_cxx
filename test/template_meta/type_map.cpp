

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
			void add()
			{
				std::string key = typeid(T).name();
				m_map[key] = std::any(T{});
			}

			template<typename T>
			T& get()
			{
				std::string key = typeid(T).name();
				if (!m_map.count(key))
				{
					add<T>();
				}
				return std::any_cast<T&>(m_map[key]);
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









#define EVAL(...) EVAL256(__VA_ARGS__)
#define EVAL256(...) EVAL128(EVAL128(__VA_ARGS__))
#define EVAL128(...) EVAL64(EVAL64(__VA_ARGS__))
#define EVAL64(...) EVAL32(EVAL32(__VA_ARGS__))
#define EVAL32(...) EVAL16(EVAL16(__VA_ARGS__))
#define EVAL16(...) EVAL8(EVAL8(__VA_ARGS__))
#define EVAL8(...) EVAL4(EVAL4(__VA_ARGS__))
#define EVAL4(...) EVAL2(EVAL2(__VA_ARGS__))
#define EVAL2(...) EVAL1(EVAL1(__VA_ARGS__))
#define EVAL1(...) __VA_ARGS__


#define SEQ_1(Mb,M) Mb(1)
#define SEQ_2(Mb,M) SEQ_1(Mb,M) M(2)
#define SEQ_3(Mb,M) SEQ_2(Mb,M) M(3)
#define SEQ_4(Mb,M) SEQ_3(Mb,M) M(4)
#define SEQ_5(Mb,M) SEQ_4(Mb,M) M(5)
#define SEQ_6(Mb,M) SEQ_5(Mb,M) M(6)
#define SEQ_7(Mb,M) SEQ_6(Mb,M) M(7)
#define SEQ_8(Mb,M) SEQ_7(Mb,M) M(8)
#define SEQ_9(Mb,M) SEQ_8(Mb,M) M(9)
#define SEQ_10(Mb,M) SEQ_9(Mb,M) M(10)
#define SEQ_11(Mb,M) SEQ_10(Mb,M) M(11)
#define SEQ_12(Mb,M) SEQ_11(Mb,M) M(12)
#define SEQ_13(Mb,M) SEQ_12(Mb,M) M(13)
#define SEQ_14(Mb,M) SEQ_13(Mb,M) M(14)
#define SEQ_15(Mb,M) SEQ_14(Mb,M) M(15)
#define SEQ_16(Mb,M) SEQ_15(Mb,M) M(16)
#define SEQ_17(Mb,M) SEQ_16(Mb,M) M(17)
#define SEQ_18(Mb,M) SEQ_17(Mb,M) M(18)
#define SEQ_19(Mb,M) SEQ_18(Mb,M) M(19)
#define SEQ_20(Mb,M) SEQ_19(Mb,M) M(20)
#define SEQ_21(Mb,M) SEQ_20(Mb,M) M(21)
#define SEQ_22(Mb,M) SEQ_21(Mb,M) M(22)
#define SEQ_23(Mb,M) SEQ_22(Mb,M) M(23)
#define SEQ_24(Mb,M) SEQ_23(Mb,M) M(24)
#define SEQ_25(Mb,M) SEQ_24(Mb,M) M(25)
#define SEQ_26(Mb,M) SEQ_25(Mb,M) M(26)
#define SEQ_27(Mb,M) SEQ_26(Mb,M) M(27)
#define SEQ_28(Mb,M) SEQ_27(Mb,M) M(28)
#define SEQ_29(Mb,M) SEQ_28(Mb,M) M(29)
#define SEQ_30(Mb,M) SEQ_29(Mb,M) M(30)

#define SEQ_N(N,Mb,M) SEQ_##N(Mb,M)

#define _SEQ_N() SEQ_N

#define EMP()
#define DEFF(m) m EMP()

#define R_SEQ_N(N,Mb,M) DEFF(_SEQ_N)()(N,Mb,M)



		///////////////////
#define VV_B(n) v##n
#define VV(n) ,v##n

#define VAR_LIST(n) R_SEQ_N(n,VV_B,VV)


#define IF_BRANCH(num)\
if constexpr (get_field_num<T>::value ==num){ \
		auto [VAR_LIST(num)] = T{}; \
		return std::make_tuple(VAR_LIST(num)); \
}\

#define ELSE_IF_BRANCH(num)\
else if constexpr (get_field_num<T>::value ==num){ \
		auto [VAR_LIST(num)] = T{}; \
		return std::make_tuple(VAR_LIST(num)); \
}\



#define TOTUPLES(n) SEQ_N(n,IF_BRANCH,ELSE_IF_BRANCH)



		template<typename T>
		constexpr auto as_tuple()
		{
			EVAL( TOTUPLES(30) );
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



#include "gmock/gmock.h"
#include <iostream>
#include <memory>

#include <string>


using namespace testing;

//namespace multi_method
namespace _0
{

    template<typename... P>
	struct type_list {};


	//front
	template<typename L >
	struct front_imp;

    template<typename H, typename... T>
	struct front_imp<type_list<H, T...>>
	{
		using type = H;
	};

	template<typename L>
	using front = typename front_imp<L>::type;

	//pop front
	template<typename L>
	struct pop_front_imp;

    template<typename H, typename... T>
	struct pop_front_imp<type_list<H, T...>>
	{
		using type = type_list<T...>;
	};

	template<typename L>
	using pop_front = typename pop_front_imp<L>::type;

	//push front
	template<typename H,typename L >
	struct push_front_imp;

	template<typename H, typename... T>
	struct push_front_imp<H, type_list< T...>>
	{
		using type = type_list<H,T...>;
	};

	template<typename H,typename L>
	using push_front = typename push_front_imp<H, L>::type;

	//get_nth_element
	template<typename L,size_t n> 
	struct get_nth_element_imp
	{
		using type = typename get_nth_element_imp<pop_front<L>, n - 1>::type;
	};

	template<typename L> 
	struct get_nth_element_imp<L,0>
	{
		using type = front<L>;
	};

	template<typename L,size_t n> 
	using get_nth_element = typename get_nth_element_imp<L, n>::type;

	//get_nth_element
	template<typename L>
	struct is_empty_imp
	{
		static inline constexpr  bool value = false;
	};

	template<>
	struct is_empty_imp<type_list<>>
	{
		static inline constexpr  bool value = true;
	};

	template<typename L>
	inline constexpr bool is_empty = is_empty_imp<L>::value;







/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	struct Shape
	{
		virtual ~Shape() {};
	};

	struct Triangle:public Shape
	{

	};

	struct Square:public Shape
	{

	};

	struct Circle:public Shape
	{

	};


	struct IntersectionCmoputer
	{
		std::string operator()(Circle* c0, Circle* c1) { return "cc"; }
		std::string operator()(Square* c0, Circle* c1) { return "sc"; }
		std::string operator()(Triangle* c0, Square* c1) { return "ts"; }

		std::string operator()(Shape* s0, Shape* s1) { return "default"; }
	};




	template<
		typename Base_Left,
		typename List_Left,
		typename Executor,
		typename R = void,
		typename Base_Right = Base_Left,
		typename List_Right = List_Left
	>
	class Static_Dispatcher
	{

	public:
		template<typename T>
		static void print_type() 
		{
			std::cout << typeid(T).name() << std::endl;
		}

		template<typename T>
		static void print_type(T v) 
		{
			std::cout << typeid(v).name() << std::endl;
		}

		static R  Go(Base_Left* l, Base_Right* r,Executor exec)
		{

			using left_current_t = front<List_Left>;

			if (left_current_t * p = dynamic_cast<left_current_t *>(l))
			{

				return Static_Dispatcher< Base_Left, type_list<>, Executor, R, Base_Right, List_Right>::Dispatch_Right(p, r, exec);

			}
			else
			{
				using List_Left_Tail = pop_front<List_Left>;

				if constexpr (is_empty<List_Left_Tail>)
				{
					return R{};//don't find
				}
				else{
					return Static_Dispatcher< Base_Left, List_Left_Tail, Executor, R, Base_Right, List_Right>::Go(l, r, exec);
				}

			}

		}

		template<typename Left_T>
		static R Dispatch_Right( Left_T* l,  Base_Right* r, Executor exec)
		{
			using right_current_t = front<List_Right>;

			if (auto p = dynamic_cast<right_current_t*>(r))
			{
//				print_type<Base_Right>();
//				print_type<Left_T>();
//				print_type<right_current_t>();
//				print_type(*l);
//				print_type(*p);

				return exec(l, p);
				//return R{};
			}
			else
			{

				using List_Right_Tail = pop_front<List_Right>;

//				print_type<right_current_t>();
//				print_type<List_Right>();
//				print_type<List_Right_Tail>();

				if constexpr (is_empty<List_Right_Tail>)
				{
					return R{};//don't find
				}
				else  
				{
					return Static_Dispatcher< Left_T, type_list<>, Executor, R, Base_Right, List_Right_Tail>::Dispatch_Right(l, r, exec);
				}

			}

		}

	};







	TEST(Multi_Method_Test, use)
	{

		IntersectionCmoputer computer;

		using Shapes = type_list<Triangle, Square, Circle>;

		std::unique_ptr<Shape> t = std::make_unique<Triangle>();
		std::unique_ptr<Shape> s = std::make_unique<Square>();

		auto act = Static_Dispatcher< Shape, Shapes, IntersectionCmoputer, std::string>::Go(t.get(), s.get(), computer);

		EXPECT_THAT(act, Eq("ts"));



	}





}

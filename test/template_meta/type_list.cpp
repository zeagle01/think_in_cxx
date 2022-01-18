#include "gmock/gmock.h"

//#include <type_traits>

using namespace testing;




namespace type_list
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

	//merge
	template<typename tl0,typename tl1>
	struct merge_list_imp;

	template<typename ...P0, typename... P1>
	struct merge_list_imp<type_list<P0...>, type_list<P1...>>
	{
		using type = type_list<P0..., P1...>;
	};

	template<typename tl0,typename tl1>
	using merge_list = merge_list_imp<tl0, tl1>::type;

	//is_empty
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




	class Type_List_Test:public Test
	{
	protected:

		using m_type_list = type_list<int, float>;


		template<typename act,typename exp>
		void type_is_same()
		{
			//bool asst = std::is_same_v< act, exp>;
			EXPECT_THAT(typeid(act).name(), Eq(typeid(exp).name()));
		}

	};




    TEST_F(Type_List_Test,get_front_test)
    {
		using act = front<m_type_list>;

		using exp = int;

		type_is_same<act, exp>();
    }

    TEST_F(Type_List_Test,pop_front_test)
    {
		using act = pop_front<m_type_list>;

		using exp = type_list<float>;

		type_is_same<act, exp>();
    }


    TEST_F(Type_List_Test,push_front_test)
    {
		using act = push_front<double,m_type_list>; 

		using exp = type_list<double,int,float>;

		type_is_same<act, exp>();
    }

	TEST_F(Type_List_Test, get_2nd_type)
    {
		using act = get_nth_element< m_type_list, 1>;

		using exp = float;

		type_is_same<act, exp>();
    }

	TEST_F(Type_List_Test, empty_list_is_empty)
    {

		EXPECT_TRUE(is_empty<type_list<>>);
    }

	TEST_F(Type_List_Test, non_list_not_is_empty)
    {

		EXPECT_FALSE(is_empty<type_list<int>>);
    }

	TEST_F(Type_List_Test, merge_list_test)
    {
		using tl0 = type_list<int>;
		using tl1 = type_list<float>;

		using act = merge_list<tl0, tl1>;
		using exp = type_list<int, float>;

		type_is_same<act, exp>();
    }



}

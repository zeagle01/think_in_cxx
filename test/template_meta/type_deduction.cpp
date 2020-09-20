
#include "gmock/gmock.h"

using namespace testing;



namespace type_deduction
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









	////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	class Type_Deduction_Test :public Test
	{
	protected:




		/////////////////////////////
		template<typename T>
		struct value
		{
			T operator()()
			{
				return a;
			}
			T a{};
		};

		template<typename T>
		struct const_value
		{
			const T operator()()
			{
				return a;
			}
			T a{};
		};

		template<typename T>
		struct pointer
		{
			T* operator()()
			{
				return &a;
			}
			T a{ };
		};

		template<typename T>
		struct reference
		{
			T& operator()()
			{
				return a;
			}
			T a{};
		};

		template<typename T>
		struct const_reference
		{
			const T& operator()() 
			{
				return a;
			}
			T a = 0;
		};

		/////////////////////////////
		struct value_template
		{
			template<typename target_type, typename source_type>
			static bool invoke(source_type src)
			{
				return std::is_same_v<source_type, target_type>;
			}
		};

		struct reference_template
		{
			template<typename target_type, typename source_type>
			static bool invoke(source_type& src)
			{
				return std::is_same_v<source_type, target_type>;
			}
		};

		struct const_reference_template
		{
			template<typename target_type, typename source_type>
			static bool invoke(const source_type& src)
			{
				return std::is_same_v<source_type, target_type>;
			}
		};

		struct forward_reference_template
		{
			template<typename target_type, typename source_type>
			static bool invoke(source_type&& src)
			{
				return std::is_same_v<source_type, target_type>;
			}
		};


		/////////////////////////////////////////

		template<typename T>
		using bind_to_value = T;

		template<typename T>
		using bind_to_const_value = const T;

		template<typename T>
		using bind_to_pointer = T*;

		template<typename T>
		using bind_to_refence = T&;

		template<typename T>
		using bind_to_const_refence = const T&;

		template<typename T>
		using bind_to_right_refence = T&&;




		/////////////////////////////////////////
		template<typename test_records >
		static void test_all()
		{

			if constexpr (!is_empty <test_records>)
			{
				using test_record = front<test_records>;

				//using base_t = get_nth_element<test_record, 0>;// not use here

				using var_t = get_nth_element<test_record, 0>;
				var_t var;

				using template_t = get_nth_element<test_record, 1>;
				using exp_t = get_nth_element<test_record, 2>;

				EXPECT_TRUE(template_t::invoke<exp_t>(var()));

			}
			using poped_test_records = pop_front<test_records>;
			if constexpr (!is_empty <poped_test_records>)
			{
				return test_all < poped_test_records >();
			}

		}

		template< typename T, typename vt, template <typename > typename vv, template <typename > typename bt >
		using test_recored = type_list< vv<T>, vt, bt<T> >;

	};

	TEST_F(Type_Deduction_Test, test_all_conbination)
	{

		using test_recoreds = type_list<
			test_recored<int, value_template, value, bind_to_value>,
			test_recored<int, value_template, pointer, bind_to_pointer>,
			test_recored<int, value_template, reference, bind_to_value>,

			test_recored<int, const_reference_template, const_value, bind_to_value>, //!!! still work
			test_recored<int, const_reference_template, value, bind_to_value>, //!!! still work
			test_recored<int, const_reference_template, reference, bind_to_value>,
			test_recored<int, const_reference_template, const_reference, bind_to_value>,

			test_recored<int, reference_template, const_reference, bind_to_const_value>,
			test_recored<int, reference_template, reference, bind_to_value>,
			//test_recored<int, reference_template, const_value, bind_to_value>,//dont compile
			//test_recored<int, reference_template, value, bind_to_value>,//dont compile

			test_recored<int, forward_reference_template, value, bind_to_value>,
			test_recored<int, forward_reference_template, reference, bind_to_refence>,
			test_recored<int, forward_reference_template, const_reference, bind_to_const_refence>,
			test_recored<int, forward_reference_template, pointer, bind_to_pointer>
		>;

		test_all<test_recoreds>();

	}



	/**
	 * template<typename T>
	 * void f(P(T) expr)
	 *
	 *  
	 *  P in{ pointer/reference  , &&, not first two}
	 *  E expression type
	 *  T template parameter
	 */


}



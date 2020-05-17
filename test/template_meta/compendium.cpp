
#include "gmock/gmock.h"
#include <type_traits>
#include <typeinfo>

// CppCon 14

namespace compendium
{


	class No_Copyable
	{
	public:
		No_Copyable(const No_Copyable&) = delete;
		No_Copyable& operator=(const No_Copyable&) = delete;

	};















//------------------------------------------
	template<int N>
	struct my_abs
	{
		static_assert(N != INT_MIN);
		//return for meta function
		static constexpr int value = (N < 0) ? -N : N;

	};

	constexpr int const_abs(int N)
	{
		return (N < 0) ? -N : N;
	}

//------------------------------------------

	template <int N>
	struct Sum
	{
		static constexpr int value = Sum<N - 1>::value + N;
	};

	template<>
	struct Sum<1>
	{
		static constexpr int value = 1;
	};

//------------------------------------------



	template<typename T>
	struct rank
	{
		static constexpr size_t value = 0u;
	};

	template<typename T,size_t N>
	struct rank<T[N]>
	{
		static constexpr size_t value = rank<T>::value + 1u;
	};


//------------------------------------------

	template<typename T>
	struct remove_const
	{
		using type = T;
	};

	template<typename T>
	struct remove_const<T const>
	{
		using type = T;
	};

//------------------------------------------

	template<typename T>
	struct type_is
	{
		using type = T;
	};

	template<typename T>
	struct remove_volatile :public type_is<T> {};

	template<typename T>
	struct remove_volatile<T volatile> : public type_is<T> {};

//------------------------------------------
	template<bool ,typename T,typename >
	struct IF :type_is<T> {};

	template<typename T,typename F>
	struct IF<false, T, F> :public type_is<F> {};

	template<bool test,typename T,typename F>
	using IF_t = typename  IF<test, T, F>::type;

//------------------------------------------
	template<bool test,typename T>
	struct enable_if :public type_is<T> {};

	template<typename T>
	struct enable_if <false, T> :type_is<void> {};


//------------------------------------------
	template<typename T,T v>
	struct integral_constant
	{
		static constexpr T value = v;

		constexpr  operator T() const noexcept { return value; }
		constexpr T operator()() const noexcept { return value; }

	};

	template<typename T>
	struct rank1 :public integral_constant<size_t, 0> {};

	template<typename T, int N>
	struct rank1<T[N]> :public integral_constant<size_t, 1 + rank1<T>()> {};

	template<typename T>
	struct rank1<T[]> :public integral_constant<size_t, 1 + rank1<T>()> {};


//------------------------------------------
	template<bool B>
	using bool_constant = integral_constant<bool, B>;
	using true_type = bool_constant<true>;
	using false_type = bool_constant<false>;


//------------------------------------------
	template<typename T>
	struct is_void :public false_type {};

	template<>
	struct is_void<void> :public true_type {};

	template<>
	struct is_void<void const> :public true_type {};



//------------------------------------------

	template<typename T1,typename T2>
	struct is_same:public false_type{};

	template<typename T>
	struct is_same<T,T>:public true_type{};


	template<typename T>
	struct is_void1 :public is_same<std::remove_cv_t<T>, void> {};


//------------------------------------------
	template<typename T,typename... T0toN>
	struct is_one_of;

	template<typename T>
	struct is_one_of<T> :public false_type {};

	template<typename T,typename... T1toN>
	struct is_one_of<T,T,T1toN...> :public true_type {};

	template<typename T,typename T0,typename... T1toN>
	struct is_one_of<T, T0, T1toN...> :public is_one_of<T, T1toN...> {};


//------------------------------------------
	template<typename T>
	struct is_copy_assignable
	{
	private:
		template< typename U, typename 
			= decltype(std::declval<U&>()= std::declval<const U&>()) >

		static true_type
			try_assign(U&&);

		static false_type
			try_assign(...);

	public:
		using type = decltype(try_assign(std::declval<T>()));
	};

}



using namespace testing;

template<typename T>
std::string get_type_string()
{
	return std::string(typeid(T).name());
}


TEST(Compendium, call_abs)
{
	//call is "request for published value"
	EXPECT_THAT(compendium::my_abs<100>::value, Eq(100));
	EXPECT_THAT(compendium::my_abs<-100>::value, Eq(100));

}

TEST(Compendium, constexpr_abs)
{
	EXPECT_THAT(compendium::const_abs(100), Eq(100));
	EXPECT_THAT(compendium::const_abs(-100), Eq(100));

}

TEST(Compendium, recursion_sum)
{
	EXPECT_THAT(compendium::Sum<3>::value, Eq(6));
	EXPECT_THAT(compendium::Sum<4>::value, Eq(10));
}


TEST(Compendium, my_rank)
{
	EXPECT_THAT(compendium::rank<int>::value, Eq(0));
	EXPECT_THAT(compendium::rank<int[1]>::value, Eq(1));
	EXPECT_THAT(compendium::rank<int[1][1]>::value, Eq(2));

	EXPECT_THAT(std::rank<int>::value, Eq(0));
	EXPECT_THAT(std::rank<int[1]>::value, Eq(1));
	EXPECT_THAT(std::rank<int[1][1]>::value, Eq(2));
}


TEST(Compendium, remove_const)
{
	EXPECT_THAT(get_type_string<compendium::remove_const<int const>::type>(), Eq("int"));
	EXPECT_THAT(get_type_string<compendium::remove_const<int>::type>(), Eq("int"));
}


TEST(Compendium, remove_volatile)
{
	EXPECT_THAT(get_type_string<compendium::remove_volatile<int volatile>::type>(), Eq("int"));
}

TEST(Compendium, template_if)
{
	auto type_string = get_type_string<compendium::IF_t<true, int, unsigned int>>();
	EXPECT_THAT(type_string, Eq("int"));


	//std conterpart
	type_string = get_type_string<std::conditional<true, int, unsigned int>::type>();
	EXPECT_THAT(type_string, Eq("int"));
}

TEST(Compendium, my_enable_if)
{
	auto type_string = get_type_string<compendium::enable_if<true, int>::type>();
	EXPECT_THAT(type_string, Eq("int"));

	type_string = get_type_string<compendium::enable_if<false, int>::type>();
	EXPECT_THAT(type_string, Eq("void"));

}

TEST(Compendium, value_meta_function)
{
	EXPECT_THAT(compendium::rank1<int[1][1]>::value, Eq(2));
	EXPECT_THAT(compendium::rank1<int[]>::value, Eq(1));
}


TEST(Compendium, is_void)
{

	EXPECT_THAT(compendium::is_void<void>(), Eq(true));
	EXPECT_THAT(compendium::is_void<int>(), Eq(false));

	EXPECT_THAT(compendium::is_void1<int>(), Eq(false));
	EXPECT_THAT(compendium::is_void1<void>(), Eq(true));

}

TEST(Compendium, is_same)
{
	EXPECT_THAT((compendium::is_same<int, float>()), Eq(false));
	EXPECT_THAT((compendium::is_same<int, int>()), Eq(true));
}


TEST(Compendium, is_one_of)
{
	EXPECT_THAT((compendium::is_one_of<int, float, int, double>()), Eq(true));
	EXPECT_THAT((compendium::is_one_of<int, int,float>()), Eq(true));
	EXPECT_THAT((compendium::is_one_of<int, double,float>()), Eq(false));

}

TEST(Compendium, is_copy_assignable)
{

	EXPECT_THAT(compendium::is_copy_assignable<int>::type::value, Eq(true));
	EXPECT_THAT(compendium::is_copy_assignable<compendium::No_Copyable>::type::value, Eq(false));

}

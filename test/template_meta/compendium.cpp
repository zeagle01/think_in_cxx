
#include "gmock/gmock.h"
#include <type_traits>

// CppCon 14

namespace compendium
{
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



}



using namespace testing;


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
	EXPECT_THAT(std::rank<int[1][1]>::value, Eq(3));
}



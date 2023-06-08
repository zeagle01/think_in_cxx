
#include "gmock/gmock.h" 

using namespace testing;


template<int N>
struct Fibnaci
{
	static inline constexpr int value = Fibnaci<N - 1>::value + Fibnaci<N - 2>::value;
};

template<>
struct Fibnaci<1>
{
	static inline constexpr int value = 1;
};

template<>
struct Fibnaci<2>
{
	static inline constexpr int value = 1;
};


TEST(Fibnacil_Test, compute_factorial)
{
	EXPECT_THAT(Fibnaci<3>::value, Eq(2));
	EXPECT_THAT(Fibnaci<5>::value, Eq(5));
}


constexpr int fib(int n)
{
	if ( n == 1)
	{
		return 1;
	}
	else if (n == 2)
	{
		return 1;
	}
	else
	{
		return fib(n - 1) + fib(n - 2);
	}
}

TEST(Fibnacil_Test, compute_factorial_with_constexpr_function)
{
	constexpr int b = fib(3); //compile time values
	EXPECT_THAT(b, Eq(2));
}



template<int ...n> 
struct Sum
{
	static inline constexpr int value = (n + ... + 0);
};


TEST(Folding_Expression_Test, compute_sum)
{
	constexpr int sum = Sum<1, 3, 5>::value;

	EXPECT_THAT(sum, Eq(9));
}

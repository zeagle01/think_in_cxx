

#include "gmock/gmock.h"

#include <random>


namespace random_test
{
	//can work but not good: for subtle reason
	int	non_standard_way()
	{
		static std::default_random_engine e;
		return (e() + 1) % 6;
	}

	int	non_standard_way_with_seed()
	{
		static std::default_random_engine e{ 0 };
		return (e() + 1) % 6;
	}

}



using namespace testing;


TEST(RandomTest, generate_int)
{
	std::random_device rd;//serve as random seed
	std::default_random_engine re;//random generator
	re.seed(10);
	std::mt19937 mt{ rd() }; //random generator
	std::uniform_int_distribution<int> dist(0, 10); //distribution
	int n = 10;
	std::vector<int> act(n);
	for (int i = 0; i < n; i++)
	{
		act[i] = dist(mt);

		EXPECT_THAT(act[i], Ge(0));
		EXPECT_THAT(act[i], Le(10));
	}
}


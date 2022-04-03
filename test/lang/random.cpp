

#include "gmock/gmock.h"

#include <random>

using namespace testing;

namespace random_test
{

	TEST(RandomTest, default_engine_test)
	{
		std::default_random_engine re;
		int n = 10;
		std::vector<int> act(n);
		for (int i = 0; i < n; i++)
		{
			act[i] = (re() + 1) % 10;

			EXPECT_THAT(act[i], Ge(0));
			EXPECT_THAT(act[i], Le(n));
		}
	}

	TEST(RandomTest, generate_int_with_mt_and_device_seed)
	{
		std::random_device rd;//serve as random seed
		
		std::mt19937 mt{ rd() }; //random generator
		std::uniform_int_distribution<int> dist(0, 10); //distribution
		int n = 10;
		std::vector<int> act(n);
		for (int i = 0; i < n; i++)
		{
			act[i] = dist(mt);

			EXPECT_THAT(act[i], Ge(0));
			EXPECT_THAT(act[i], Le(n));
		}
	}

	TEST(RandomTest, generate_int_with_seed)
	{
		
		std::default_random_engine re;//random generator
		re.seed(10);
		std::uniform_int_distribution<int> dist(0, 10); //distribution
		int n = 10;
		std::vector<int> act(n);
		for (int i = 0; i < n; i++)
		{
			act[i] = dist(re);

			EXPECT_THAT(act[i], Ge(0));
			EXPECT_THAT(act[i], Le(n));
		}
	}

}







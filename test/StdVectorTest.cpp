#include "gtest/gtest.h"





TEST(StdVectorTest, test_unique) {


	std::vector<int> act{2,1,1,1, 3, 3};

	std::vector<int> exp{1, 2, 3};


	std::sort(std::begin(act), std::end(act));
	auto &unique_end=std::unique(std::begin(act), std::end(act));

	act.erase(unique_end, std::end(act));
	EXPECT_EQ(act, exp);


}

#include "gmock/gmock.h"

#include <vector>
#include <type_traits>
#include <algorithm>

using namespace testing;



TEST(StdVectorTest, test_unique) {


	std::vector<int> act{2,1,1,1, 3, 3};

	std::vector<int> exp{1, 2, 3};


	std::sort(std::begin(act), std::end(act));
	auto &unique_end=std::unique(std::begin(act), std::end(act));

	act.erase(unique_end, std::end(act));
	EXPECT_EQ(act, exp);


}

TEST(StdVectorTest, test_remove_extent) {



//	std::vector<int> act{2,1,1,1, 3, 3};

	//won't work
//	std::remove_extent<std::vector<int>>::type act=1;
	std::vector<int>::value_type act = 1;
	int exp =1;


	EXPECT_EQ(act, exp);


}

TEST(StdVectorTest, test_erase_given_index) 
{
	std::vector<int> act{ 1,2,3 };
	//remove
	act.erase(act.begin() + 1);
	std::vector<int> exp{ 1,3 };
	EXPECT_EQ(act, exp);
}

TEST(StdVectorTest, test_remove_given_element) 
{
	std::vector<int> act{ 1,2,3 };
	//remove
	std::remove(act.begin(), act.end(), 2);
	std::vector<int> exp{ 1,3,3};
	EXPECT_EQ(act, exp);
}

TEST(StdVectorTest, test_remove_two_given_element) 
{
	std::vector<int> act{ 1,2,3,2,4 };
	//remove
	std::remove(act.begin(), act.end(), 2);//unspecifed!
	std::vector<int> exp{ 1,3,4,2,4 };
	EXPECT_EQ(act, exp);
}

TEST(StdVectorTest, test_erase_given_element) 
{
	std::vector<int> act{ 1,2,3 };
	//remove
	auto it = std::remove(act.begin(), act.end(), 2);
	act.erase(it, act.end());
	std::vector<int> exp{ 1,3 };
	EXPECT_EQ(act, exp);
}


TEST(StdVectorTest, resize_will_not_override_with_same_size) 
{
	std::vector<int> act{ 1,2,3 };
	act.resize(3, 0);
	std::vector<int> exp{ 1,2,3 };
	EXPECT_EQ(act, exp);
}

TEST(StdVectorTest, resize_will_not_override_with_different_size) 
{
	std::vector<int> act{ 1,2,3 };
	act.resize(2, 0);
	std::vector<int> exp{ 1,2 };
	EXPECT_THAT(act, Eq(exp));
}

TEST(StdVectorTest, resize_will_init_with_given_value_when_exceed_original_size) 
{
	std::vector<int> act{ 1,2 };
	act.resize(3, 5);
	std::vector<int> exp{ 1,2 ,5 };
	EXPECT_THAT(act, Eq(exp));
}

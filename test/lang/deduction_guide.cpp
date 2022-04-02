
#include "gmock/gmock.h"
#include <type_traits>

using namespace testing;

namespace deduction_guide
{


	template<typename T = int>
	struct A {
		T d;
	};

	template<typename T >
	struct B {
		T d;
	};

	template<typename T>
	B() -> B<int>; //all go to int


	TEST(Deduction_Guide_Test, default_decleration)
	{

		A a;
		bool is_same_type = std::is_same_v<decltype(a.d), int>;
		EXPECT_TRUE(is_same_type);

	}


	TEST(Deduction_Guide_Test, use_guide)
	{
		B<float> b;

		bool is_same_type = std::is_same_v<decltype(b.d), int>;
		EXPECT_TRUE(is_same_type);

	}
}

#include "gmock/gmock.h"

#include <string>

using namespace testing;

namespace  partial_specialization_2_dispatch
{
	struct type_groups
	{
		struct A {};
		struct B {};
		struct C {};
	};

	template<typename T>
	struct  output;

	template<>
	struct  output<type_groups::A>
	{
		std::string operator ()()
		{
			return "A";
		}
	};

	template<>
	struct  output<type_groups::B>
	{
		std::string operator ()()
		{
			return "B";
		}
	};

	template<>
	struct  output<type_groups::C>
	{
		std::string operator ()()
		{
			return "C";
		}
	};

	TEST(another_test, dispatch)
	{
		EXPECT_THAT(output<type_groups::A>()(), Eq("A"));
		EXPECT_THAT(output<type_groups::B>()(), Eq("B"));
		EXPECT_THAT(output<type_groups::C>()(), Eq("C"));
	}


}


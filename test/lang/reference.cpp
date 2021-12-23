
#include "gmock/gmock.h"

using namespace testing;


namespace reference_test
{

	TEST(Reference_Test, const_refence_changed_by_variable_itself)
	{

		int a = 1;
		const int& ra = a;

		a = 2;
		EXPECT_THAT(ra, Eq(2));
	}


}

#include "gmock/gmock.h"

import math;

using namespace testing;

namespace module_test
{

	TEST(Module_Test, use_exported_free_function)
	{
		EXPECT_THAT(add(1, 1), 2);
	}


}
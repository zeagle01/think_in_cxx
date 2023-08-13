
#include "gmock/gmock.h"

import math;

using namespace testing;

namespace module_test
{

	TEST(Module_Test, use_exported_free_function)
	{
		EXPECT_THAT(add(1, 1), 2);
	}

	TEST(Module_Test, function_that_hide_implementation)
	{
		EXPECT_THAT(func_hide_imp(2, 1), 1);
	}

	TEST(Module_Test, use_exported_template_function)
	{
		EXPECT_THAT(a_tmp_func(1), 1);
	}

	TEST(Module_Test, template_function_that_hide_implementation)
	{
		EXPECT_THAT(a_tmp_func_hide_imp(1), 1);
	}

	TEST(Module_Test, template_function_that_hide_implementation_in_partition_unit)
	{
		EXPECT_THAT(a_tmp_func_hide_imp_in_partition_unit(1), 1);
	}


}

#include "gmock/gmock.h"

using namespace testing;


namespace type_parameter_test
{




	template<typename T>
	class ArithmiticTest:public Test
	{
	public:
		
	};

	TYPED_TEST_SUITE_P(ArithmiticTest);

	TYPED_TEST_P(ArithmiticTest, postive_add_negative_is_zero)
	{
		TypeParam a0 = 2;
		TypeParam a1 = -2;

		EXPECT_THAT(a0 + a1, Eq(0));
	}


	REGISTER_TYPED_TEST_SUITE_P(ArithmiticTest, postive_add_negative_is_zero);



	using my_types = testing::Types<float, double, int, short int ,char>;
	INSTANTIATE_TYPED_TEST_SUITE_P(My, ArithmiticTest, my_types);

}
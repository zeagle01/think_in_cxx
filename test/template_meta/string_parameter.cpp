
#include "gmock/gmock.h"

using namespace testing;


namespace string_parameter
{

	template<int  N>
	struct literal_string
	{
		constexpr literal_string(char const(&a)[N] )  
		{
			std::copy_n(a, N, data);
		};

		char data[N];
	};



	template<literal_string s>
	int config;

	template<>
	int config<"a"> = 0;

	template<>
	int config<"b"> = 1;

	template<>
	int config<"c"> = 2;

	template<>
	int config<"abc"> = 3;


	template<literal_string>
	struct A
	{

	};

	TEST(string_parameter, test_string_to_int_map)
	{
		EXPECT_THAT(config<"a">, Eq(0));
		EXPECT_THAT(config<"b">, Eq(1));
		EXPECT_THAT(config<"c">, Eq(2));

		EXPECT_THAT(config<"abc">, Eq(3));
	}

}

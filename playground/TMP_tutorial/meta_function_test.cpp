
#include "gmock/gmock.h" 

using namespace testing;

template<typename T>
struct Is_Int
{
	static inline constexpr bool  value = false;
};

template<>
struct Is_Int<int>
{
	static inline constexpr bool  value = true;
};

TEST(Meta_Function_Test, meta_function_that_check_if_a_type_is_int)
{
	EXPECT_TRUE(Is_Int<int>::value);
	EXPECT_FALSE(Is_Int<float>::value);
}

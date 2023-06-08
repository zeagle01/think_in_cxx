
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


template<typename T>
struct Identity { using type = T; };

TEST(Meta_Function_Test, identy_return_given_type)
{
	using U = Identity<int>::type; //calling the function
	bool v = std::is_same_v<int, U>;
	EXPECT_TRUE(v);
}

template<typename T> // parameter
struct Type_Size {
	static inline constexpr int  value = sizeof(T);  //value, the return 
};

TEST(Meta_Function_Test, get_size_of_int)
{
	int int_size = Type_Size<int>::value; //calling the function
	EXPECT_THAT(int_size,Eq(4));
}

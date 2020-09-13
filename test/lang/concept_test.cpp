


#include "gmock/gmock.h"


#include <concepts>



using namespace testing;


namespace concept_test
{

	template<typename T>
	concept has_type = requires {typename T::type; };


	struct A
	{};


	struct B
	{
		using type = B;
	};


	TEST(Concept_Test, has_type)
	{
		//auto a = has_type<A<void>>;

		EXPECT_THAT(has_type<A>, Eq(false));
		EXPECT_THAT(has_type<B>, Eq(true));

	}


}
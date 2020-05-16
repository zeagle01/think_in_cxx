
#include "gmock/gmock.h"

// CppCon 14

namespace compendium
{
	template<int N>
	struct my_abs
	{
		static_assert(N != INT_MIN);
		//return for meta function
		static constexpr int value = (N < 0) ? -N : N;

	};

}



using namespace testing;
using namespace compendium;


TEST(Compendium, call_abs)
{
	//call is "request for published value"
	EXPECT_THAT(my_abs<100>::value, Eq(100));
	EXPECT_THAT(my_abs<-100>::value, Eq(100));
}




#include "gmock/gmock.h"
#include <thread>
#include <chrono>

using namespace testing;

namespace thread_basic_use
{

	TEST(Basic_Use, use)
	{

		int a = 0;
		{
			std::jthread tt(
				[&a]()
				{
					a++;
				}
			);
		}
		//std::this_thread::sleep_for(std::chrono::duration<int>(1));

		EXPECT_THAT(a, Eq(1));
	}


}
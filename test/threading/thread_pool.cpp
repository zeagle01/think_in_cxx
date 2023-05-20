
#include "gmock/gmock.h"
#include <mutex>
#include <functional>

using namespace testing;

namespace thread_pool
{
	class Thread_Pool
	{
	public:
		void add_task(std::function<void()> task)
		{
			task();
		}

	};

	class Thread_Pool_Test :public Test
	{
	protected:
		Thread_Pool pool;
	};


	TEST_F(Thread_Pool_Test, submit_a_task_and_it_is_excuted)
	{
		bool is_executed = false;;
		auto fn = [&]() 
		{
			is_executed = true;
		};

		pool.add_task(fn);

		EXPECT_TRUE(is_executed);
	}

}

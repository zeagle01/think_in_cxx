
#include "gmock/gmock.h"
#include <mutex>
#include <functional>
#include <chrono>

using namespace testing;

namespace thread_pool
{
	class Thread_Pool
	{
	public:
		void add_task(std::function<void()> task)
		{
			m_thread = std::make_unique<std::thread>(task);
		}
		~Thread_Pool()
		{
			m_thread->join();
		}
	private:
		std::unique_ptr<std::thread> m_thread;
	};

	class Thread_Pool_Test :public Test
	{
	public:
		Thread_Pool_Test()
		{
			m_execute_fn = [&]()
			{
				is_executed = true;
			};

			m_execute_then_notify_fn = [&]()
			{
				is_executed = true;
				cv.notify_one();
			};

		}

	protected:
		Thread_Pool pool;

		std::condition_variable cv;
		std::mutex m;
		std::atomic<bool> is_executed = false;

		std::function<void()> m_execute_fn;
		std::function<void()> m_execute_then_notify_fn;
	};


	TEST_F(Thread_Pool_Test, submit_a_task_and_it_is_excuted)
	{

		pool.add_task(m_execute_then_notify_fn);

		{
			std::unique_lock lk(m);
			cv.wait(lk);
		}

		EXPECT_TRUE(is_executed);

	}

	TEST_F(Thread_Pool_Test, wont_be_executed_right_away)
	{
		pool.add_task(m_execute_fn);

		EXPECT_FALSE(is_executed);

	}


}


#include "gmock/gmock.h"
#include <condition_variable>
#include <vector>
#include <string>
#include <mutex>
#include <chrono>


using namespace testing;

namespace condition_variable_test
{


	class Condition_Variable_Exmple
	{
	public:

		void run()
		{
			std::thread t0(
				[this]()
				{
					std::unique_lock lock(m_mutex);
					m_condition.wait(lock);
					m_execute_sequens.push_back("t0");
				}
			);

			std::thread t1(
				[this]()
				{
					std::this_thread::sleep_for(std::chrono::duration<float>(0.2));

					m_execute_sequens.push_back("t1");
					m_condition.notify_one();
				}
			);

			t0.join();
			t1.join();

		}

		std::vector<std::string> get_execute_sequens()
		{
			return m_execute_sequens;
		}

	private:
		std::condition_variable m_condition;
		std::mutex m_mutex;
		std::vector<std::string>  m_execute_sequens;

	};




	TEST(Condition_Variable_Test, one_thread_wait_for_another_one)
	{
		Condition_Variable_Exmple exmple; 
		exmple.run();
		auto act = exmple.get_execute_sequens();
		std::vector<std::string> exp{ "t1","t0" };
		EXPECT_THAT(exp, act);
	}


}
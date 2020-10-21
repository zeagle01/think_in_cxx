

#include "gmock/gmock.h"

#include <functional>
#include <vector>

using namespace testing;


namespace signal_slot_test
{


	template<typename ...P>
	struct slot
	{

		slot(std::function<void(P...)> f) 
		{
			m_f = f;
		}

		void operator()(P&&... p)
		{
			m_f(std::forward<P>(p)...);
		}

		std::function<void(P...)> m_f;

	};


	template<typename ...P>
	struct signal
	{



		void connect(slot<P...> slot)
		{
			slot_functions.push_back(slot);
		}

		void emit(P&&... p)
		{

			for (auto& slot_function : slot_functions)
			{
				slot_function(std::forward<P>(p)...);
			}

		}

		std::vector<std::function<void(P...)>> slot_functions;

	};


	TEST(signal_slot_test,non_parameter)
	{
		signal<> sig;
		slot<> slot = { 
			[=]()
			{
			printf("inside");
			} 
		};

		sig.connect(slot);
		sig.emit();

	}

	TEST(signal_slot_test,send_int)
	{
		signal<int&> sig;
		slot<int&> slot = 
		{
			[=](int& i) 
			{
				i = 7;
			}
		};

		sig.connect(slot);
		int a = 6;
		sig.emit(a);

		EXPECT_THAT(a, Eq(7));

	}
}
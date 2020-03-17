


#include "gmock/gmock.h"

#include <memory>




namespace state_pattern {


	class State
	{
		

	public:
		enum class Status{Running,Recording,Idle};

		virtual std::shared_ptr<State> press_start() = 0;
		virtual std::shared_ptr<State> press_record() =0;
		virtual State::Status get_status() = 0;

	};


	class Running :public State 
	{
	public:
		virtual std::shared_ptr<State> press_start() override;
		virtual std::shared_ptr<State> press_record() override;
		virtual State::Status get_status() override 
		{
			return State::Status::Running; 
		};

		static std::shared_ptr<State> get_singleton() 
		{
			static std::shared_ptr<State> sig;
			if (!sig)
			{
				sig= std::make_shared<Running>();
			}
			return sig;
		}
		
	};


	class Recording :public State 
	{
	public:

		virtual std::shared_ptr<State> press_start() override 
		{ 
			return Running::get_singleton(); 
		};
		virtual std::shared_ptr<State> press_record() override 
		{
			return Recording::get_singleton(); 
		};
		virtual State::Status get_status() override 
		{
			return State::Status::Recording; 
		};

		static std::shared_ptr<State> get_singleton() 
		{
			static std::shared_ptr<State> sig;
			if (!sig)
			{
				sig= std::make_shared<Recording>();
			}
			return sig;
		}
	};

	class Idle :public State 
	{
	public:
		virtual std::shared_ptr<State> press_start() override 
		{
			return Running::get_singleton();
		};
		virtual std::shared_ptr<State> press_record() override 
		{
			return Recording::get_singleton();
		} ;
		virtual State::Status get_status() override 
		{
			return State::Status::Idle;
		};

		static std::shared_ptr<State> get_singleton() 
		{
			static std::shared_ptr<State> sig;
			if (!sig)
			{
				sig= std::make_shared<Idle>();
			}
			return sig;
		}
	};

	class State_Holder :public State
	{
	private:
		std::shared_ptr<State> m_state = std::make_shared<Idle>();

	public:
			virtual std::shared_ptr<State> press_start() override 
			{ 
				m_state=m_state->press_start(); 
				return m_state;

			};
			virtual std::shared_ptr<State> press_record() override 
			{
				m_state=m_state->press_record();
				return m_state;
			}
			virtual State::Status get_status() override 
			{
				return m_state->get_status(); 
			};

	};


	std::shared_ptr<State> Running::press_start()  
	{
			return Running::get_singleton(); 
	}

	std::shared_ptr<State> Running::press_record()  
	{
			return Recording::get_singleton(); 
	}


}


using namespace testing;

class State_Pattern_Test : public testing::Test
{
protected:
	std::shared_ptr<state_pattern::State> a_state=std::make_shared<state_pattern::State_Holder>();

};




TEST_F(State_Pattern_Test, test_start_running)
{
	a_state->press_start();
	EXPECT_THAT(a_state->get_status(), Eq(state_pattern::State::Status::Running));

}


TEST_F(State_Pattern_Test, test_start_recording)
{

	a_state->press_record();
	EXPECT_THAT(a_state->get_status(), Eq(state_pattern::State::Status::Recording));

}

TEST_F(State_Pattern_Test, test_press_double_start_recording)
{

	a_state->press_record();
	a_state->press_record();
	EXPECT_THAT(a_state->get_status(), Eq(state_pattern::State::Status::Recording));

}


TEST_F(State_Pattern_Test, test_press_start_running_then_recording)
{

	a_state->press_start();
	a_state->press_record();
	EXPECT_THAT(a_state->get_status(), Eq(state_pattern::State::Status::Recording));

}

TEST_F(State_Pattern_Test, test_press_recording_then_running)
{

	a_state->press_record();
	a_state->press_start();
	EXPECT_THAT(a_state->get_status(), Eq(state_pattern::State::Status::Running));

}

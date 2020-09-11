


#include "gmock/gmock.h"

#include <memory>

#include <map>


using namespace testing;


namespace state_pattern {


	enum class StateType { Simulating, Recording, Idle };
	enum class ActionType { SimulationSwitch, RecordingSwitch };

	struct FromState
	{
		StateType state;
		ActionType action;

		bool operator<(const FromState& other) const
		{
			if (this->state != other.state)
			{
				return this->state < other.state;
			}
			else
			{
				return this->action < other.action;
			}
		}

	};

	class State
	{
	public:


		virtual std::shared_ptr<State> update_and_shift(ActionType action)
		{

			update(action);

			return get_instance(m_state_shift_map[{get_state(), action}]);

		};

		virtual StateType get_state() = 0;


		static std::shared_ptr<State> get_instance(StateType state);

	protected:

		State()
		{
			m_state_shift_map[{StateType::Idle, ActionType::SimulationSwitch}] = StateType::Simulating;
			m_state_shift_map[{StateType::Idle, ActionType::RecordingSwitch}] = StateType::Recording;

			m_state_shift_map[{StateType::Simulating, ActionType::SimulationSwitch}] = StateType::Idle;
			m_state_shift_map[{StateType::Simulating, ActionType::RecordingSwitch}] = StateType::Recording;

			m_state_shift_map[{StateType::Recording, ActionType::SimulationSwitch}] = StateType::Simulating;
			m_state_shift_map[{StateType::Recording, ActionType::RecordingSwitch}] = StateType::Idle;

		}

		virtual void update(ActionType action) = 0;


		std::map<FromState, StateType> m_state_shift_map;
	};



	class Recording :public State
	{
	public:

		virtual void update(ActionType action) override
		{

		}

		virtual StateType get_state() override
		{
			return StateType::Recording;
		}
	};

	class Simulating :public State
	{
	public:

		virtual  void update(ActionType action) override
		{

		}

		virtual StateType get_state() override
		{
			return StateType::Simulating;
		}
	};

	class Idle :public State
	{
	public:
		virtual void update(ActionType action) override
		{
		}

		virtual StateType get_state() override
		{
			return StateType::Idle;
		}
	};





	std::shared_ptr<State> State::get_instance(StateType state)
	{

		static std::map<StateType, std::shared_ptr<State>> instances;
		if (instances.empty())
		{
			instances[StateType::Simulating] = std::make_shared<Simulating>();
			instances[StateType::Recording] = std::make_shared<Recording>();
			instances[StateType::Idle] = std::make_shared<Idle>();
		}
		return instances[state];
	}








	struct StateShiftTestRecord
	{
		FromState from_struct;
		StateType to_state;
	};


	class State_Pattern_Test : public testing::Test, public WithParamInterface<StateShiftTestRecord>
	{
	public:
		std::shared_ptr<State> m_state;

	};










	TEST_P(State_Pattern_Test, test_start_running)
	{
		auto from_state = GetParam().from_struct.state;
		auto action = GetParam().from_struct.action;

		m_state = State::get_instance(from_state);

		m_state = m_state->update_and_shift(action);

		auto to_state = GetParam().to_state;
		EXPECT_THAT(m_state->get_state(), Eq(to_state));
	}

	INSTANTIATE_TEST_SUITE_P(My, State_Pattern_Test,
		Values(
			StateShiftTestRecord{ {StateType::Idle,ActionType::SimulationSwitch},StateType::Simulating },
			StateShiftTestRecord{ {StateType::Idle,ActionType::RecordingSwitch},StateType::Recording },

			StateShiftTestRecord{ {StateType::Simulating,ActionType::SimulationSwitch},StateType::Idle },
			StateShiftTestRecord{ {StateType::Simulating,ActionType::RecordingSwitch},StateType::Recording },

			StateShiftTestRecord{ {StateType::Recording,ActionType::SimulationSwitch},StateType::Simulating },
			StateShiftTestRecord{ {StateType::Recording,ActionType::RecordingSwitch},StateType::Idle }

		)
	);


}





#include "gmock/gmock.h"

#include <vector>
#include <string>

using namespace testing;

namespace special_methods
{

	using vstr = std::vector < std::string>;
	using ID = int;
	class Execute_History
	{
		std::vector < std::string> m_excutedFunctions;
		std::vector <ID> m_id;

	public:
		void push_back(const std::string& name,const ID id) { m_excutedFunctions.push_back(name); m_id.push_back(id); }
		void clear() { m_excutedFunctions.clear(); m_id.clear(); };

		const vstr& get_execute_function_name() { return m_excutedFunctions; }
		vstr get_execute_function_name_with_id() 
		{
			vstr ret;
			for (int i = 0; i < m_excutedFunctions.size(); i++)
			{
				ret.push_back(std::to_string(m_id[i]) + m_excutedFunctions[i]);
			}
			return ret;
		}
	};
	Execute_History s_excutedFunctions;
	static ID s_id;

	class A
	{
	public:
		A() 
		{
			s_excutedFunctions.push_back("ctor", s_id);
			m_id = s_id;
			s_id++;
		}
		~A() 
		{ 
			s_excutedFunctions.push_back("dtor", m_id); 
			s_id--; 
		}

		A(const A& other) 
		{ 
			s_excutedFunctions.push_back("copy_ctor",s_id);
			m_id = s_id;
			s_id++;
		}

		A& operator=(const A& other) 
		{ 
			s_excutedFunctions.push_back("copy",m_id);
			return *this; 
		}

		A(A&& other)
		{
			s_excutedFunctions.push_back("move_ctor",s_id);
			m_id = s_id;
			s_id++;
		}

		A& operator=(A&& other) 
		{ 
			s_excutedFunctions.push_back("move",m_id);
			return *this; 
		}
	private:
		ID m_id;
	};


	class Special_Method_Test :public Test
	{

	protected:
		void SetUp()
		{
			s_excutedFunctions.clear();
			s_id = 0;
		}
	};


	TEST_F(Special_Method_Test, constructor)
	{

		A a;
		vstr exp = { "ctor" };
		EXPECT_THAT(s_excutedFunctions.get_execute_function_name(), Eq(exp));
	}

	TEST_F(Special_Method_Test, constuct_and_then_destruct_in_block)
	{
		{
			A a;
		}
		vstr exp = { "ctor","dtor"};
		EXPECT_THAT(s_excutedFunctions.get_execute_function_name(), Eq(exp));
	}

	TEST_F(Special_Method_Test, destruct_in_reverse_order)
	{
		{
			A a;
			A a1;
		}
		vstr exp = { "0ctor","1ctor","1dtor","0dtor"};
		EXPECT_THAT(s_excutedFunctions.get_execute_function_name_with_id(), Eq(exp));
	}

	TEST_F(Special_Method_Test, copy_from_a_temp_invoke_just_one_constructor_because_copy_elision)
	{
		A a{ A{} };
		vstr exp = { "ctor"};
		EXPECT_THAT(s_excutedFunctions.get_execute_function_name(), Eq(exp));
	}

	TEST_F(Special_Method_Test, copy_construct_from_other)
	{
		A a;
		A b{ a };
		vstr exp = { "ctor","copy_ctor"};
		EXPECT_THAT(s_excutedFunctions.get_execute_function_name(), Eq(exp));
	}

	TEST_F(Special_Method_Test, copy)
	{
		A a;
		A b;
		a = b;
		vstr exp = { "ctor","ctor","copy"};
		EXPECT_THAT(s_excutedFunctions.get_execute_function_name(), Eq(exp));
	}

	TEST_F(Special_Method_Test, move)
	{
		A a;
		A b;
		a = std::move(b);
		vstr exp = { "ctor","ctor","move"};
		EXPECT_THAT(s_excutedFunctions.get_execute_function_name(), Eq(exp));
	}

	TEST_F(Special_Method_Test, move_ctor)
	{
		A a;
		A b(std::move(a));
		vstr exp = { "ctor","move_ctor"};
		EXPECT_THAT(s_excutedFunctions.get_execute_function_name(), Eq(exp));
	}



}

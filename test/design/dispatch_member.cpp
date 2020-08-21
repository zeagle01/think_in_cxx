


#include "gmock/gmock.h"
#include <string>
#include <map>
#include <sstream>


using namespace testing;

namespace dispatch_member
{

	std::string helper(std::string str, int in)
	{
			std::stringstream ss;
			ss << str << in;
			return ss.str();
	}


	class A_T
	{
	public:
		std::string call(int in) {
			return helper("A_T", in);
		}

	};

	class B_T
	{
	public:
		std::string call(int in) {
			return helper("B_T", in);
		}
	};

	class C_T
	{
	public:
		std::string call(int in) 
		{
			return helper("C_T", in);
		}
	};


	class A_F
	{
	public:
		static std::string call(int in) { 
			return helper("A_F", in);
		}

	};

	class B_F
	{
	public:
		static std::string call(int in) { 
			return helper("B_F", in);
		}
	};

	class C_F
	{
	public:
		static std::string call(int in) { 
			return helper("C_F", in);
		}
	};


	enum class My_Type
	{
		A,B,C
	};

	struct Key
	{
		My_Type type;
		bool condition;
	};
	class Client
	{
	private:
		struct Key_Less
		{
			bool operator()(const Key& l, const  Key& r) const
			{
				if (l.type != r.type)
				{
					return l.type<r.type;
				}
				else
				{
					return l.condition < r.condition;
				}
			}
		};

		A_T at;
		B_T bt;
		C_T ct;

	public:
		std::string invoke(My_Type type,bool condition,int in) {
			static std::map<Key, std::function<std::string(int)>, Key_Less>    dispatch_map;
			if (dispatch_map.empty())
			{
				dispatch_map[{My_Type::A, true}] = [&](int i) {return at.call(i); };
				dispatch_map[{My_Type::B, true}] = [&](int i) {return bt.call(i); };
				dispatch_map[{My_Type::C, true}] = [&](int i) {return ct.call(i); };
				dispatch_map[{My_Type::A, false}] = [](int i) {return A_F::call(i); };
				dispatch_map[{My_Type::B, false}] = [](int i) {return B_F::call(i); };
				dispatch_map[{My_Type::C, false}] = [](int i) {return C_F::call(i); };
			}

			return dispatch_map[{type, condition}](in);

		}

	};


	TEST(Dispatch_Member, according_to_type)
	{
		Client c;
		std::vector<Key> keys{ {My_Type::A, true},{My_Type::B, true},{My_Type::C, true},{My_Type::A, false},{My_Type::B, false},{My_Type::C, false} };
		std::vector<std::string> act;

		for (auto& k : keys)
		{
			act.push_back(c.invoke(k.type, k.condition, 1));
		}
		std::vector<std::string> exp = { "A_T1","B_T1","C_T1","A_F1","B_F1","C_F1" };

		EXPECT_THAT(act, Eq(exp));
	}


}
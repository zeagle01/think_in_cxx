
#include "gmock/gmock.h"

#include <memory>
#include <iostream>


using namespace testing;
namespace  parameter_test
{

	struct Bank_Account
	{
		int balance = 0;

		void with_draw(int amount)
		{
			if (amount <= balance)
			{
				balance -= amount;
			}
		}

		void deposit(int amount)
		{
			balance += amount;
		}

	};




	struct With_Draw_Data
	{
		int init_balance;
		int with_draw_amount;
		int final_balance;

		friend std::ostream& operator<<(std::ostream& out, const With_Draw_Data& d)
		{
			out << "init " << d.init_balance << ", with draw " << d.with_draw_amount << ", final " << d.final_balance;
			return out;
		}
	};

	struct Bank_Account_Withdraw_Test : public Test, public WithParamInterface<With_Draw_Data>
	{

		std::unique_ptr<Bank_Account> m_account;
		Bank_Account_Withdraw_Test()
		{

			m_account = std::make_unique<Bank_Account>();
			m_account->balance = GetParam().init_balance;
		}
	};

	TEST_P(Bank_Account_Withdraw_Test, more_with_draw_test)
	{
		auto param = GetParam();
		m_account->with_draw(param.with_draw_amount);
		//EXPECT_THAT(m_account->balance, Eq(param.final_balance)) << "haha " << PrintToString(param);
		EXPECT_THAT(m_account->balance, Eq(param.final_balance));
	}

	//
	INSTANTIATE_TEST_SUITE_P(Bank_Account_Withdraw_, Bank_Account_Withdraw_Test,
		Values(
			With_Draw_Data{ 0,10,0 },
			With_Draw_Data{ 10,10,0 },
			With_Draw_Data{ 20,10,10 },
			With_Draw_Data{ -10,10,-10 }
			//With_Draw_Data{ -10,0,-11 }//this one is intentialy wrong
		)
	);



}

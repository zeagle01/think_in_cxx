
#include "gtest/gtest.h"


namespace share_ptr_test 
{

	template<typename T>
	class Shared_ptr
	{

	public:

		operator bool()
		{
			return false;
		}

	};

	TEST(Share_Ptr_Test, defaul_constructed_one)
	{
		Shared_ptr<int> p;

		EXPECT_FALSE(p);
	}
}

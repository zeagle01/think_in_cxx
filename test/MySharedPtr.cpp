
#include "gtest/gtest.h"


namespace MySharedPtr {

	class A
	{
	public:
		A()
		{
			mData = new int32_t[10];
			mRefCount = new int32_t;
		}
		A(const A& other)
		{
			copy(other);
			(*mRefCount)++;
		}
		A& operator=(A& other)
		{
			copy(other);
			tryFree(other);
			return *this;
		}
		~A()
		{
			tryFree(*this);
		}

	private:
		void copy(const A& other)
		{
			mData = other.mData;
			mRefCount = other.mRefCount;
		}
		void tryFree(A& a)
		{
			(*a.mRefCount)--;
			if ((*a.mRefCount) == 0)
			{
				delete a.mData;
				delete a.mRefCount;
			}
		}
		int32_t* mData;
		int32_t* mRefCount;
	};

	TEST(MySharedPtrTest, test_use)
	{
		A a;
		A a1;
		A a2 = a;
	}
}

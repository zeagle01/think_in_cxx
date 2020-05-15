
#include "gmock/gmock.h"
#include <iostream>



namespace crtp
{
	template<typename Imp>
	class Feature
	{
	public:
		void fun() {
			printf("base fun \n");
			Imp imp;
			imp.doFun();
		}

	};

	class Do_Fun_1 :public Feature<Do_Fun_1>
	{
	public:
		void doFun()
		{
			printf("do fun 1 \n");
		}

	};

	class Do_Fun_2 :public Feature<Do_Fun_2>
	{
	public:
		void doFun()
		{
			printf("do fun 2 \n");
		}

	};


}
using namespace testing;
using namespace crtp;


TEST(crtp, use)
{
	Do_Fun_1 aa;
	aa.fun();

	Do_Fun_2 bb;
	bb.fun();

}


#include "gmock/gmock.h"

#include <vector>


using namespace testing;

namespace subclass_visit
{
	namespace _1
	{
		class Base
		{
		public:
			int a;
			void f()
			{

				printf("in base f\n");
			}

		};

		class Derived :public Base
		{
		public:
			int b;
			void g()
			{
				printf("int derived g\n");
			}
		};



		TEST(subclass_visit, use)
		{
			Base *a = new Base;;
			a->f();
			((Derived*)(a))->g();
		}
	}

	namespace _2
	{

		struct Base
		{
			int b;
		};

		struct Derived:public Base
		{
			int d;
		};



		TEST(subclass_visit, sub_non_virtual_class_vector)
		{
			std::vector<Base> bases;

			bases.push_back(Derived{ 1,2 });
			bases.push_back(Base{ 0 });


			EXPECT_THAT(bases[0].b, Eq(1));

			//EXPECT_THAT(bases[1].d, Eq(2)); //do not compiles

			auto act = ((Derived*)(&bases[1]))->d;
			EXPECT_THAT(act, Ne(2)); 
		}
	}

}






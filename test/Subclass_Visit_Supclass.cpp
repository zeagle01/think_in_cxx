

#include "gmock/gmock.h"



namespace subclass_visit
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

    class Derived:public Base
    {
	public:
        int b;
        void g()
        {
			printf("int derived g\n");
        }
    };

}

using namespace testing;
using namespace subclass_visit;


TEST(subclass_visit,use)
{
    Base *a=new Base;;
    a->f();
    ((Derived*)(a))->g();
}

#include "gmock/gmock.h"

using namespace testing;

namespace deducing_this
{


	class Base
	{
	public:
		template<typename Self>
		std::string  f(this Self& self)
		{
			return "Base+" + self.g();
		}
	};

	class A:public Base
	{
	public:
		std::string  g()
		{
			return "A";
		}
	};

	class B :public Base
	{
	public:
		std::string  g()
		{
			return "B";
		}
	};


	TEST(deducing_this_test, call_subclass_method_in_base_class)
	{
		A a;
		B b;
		EXPECT_THAT(a.f(), Eq("Base+A"));
		EXPECT_THAT(b.f(), Eq("Base+B"));

	}

}




#include "gmock/gmock.h"


#include <string>
#include <memory>

using namespace testing;


namespace  virtual_non_virtual
{


    enum class Methods
    {
	Base_Virtual,
	Base_Non_Virtual,
	Derived_Virtual,
	Derived_Non_Virtual
    };



    class Base
    {
	public:
	virtual Methods  virtual_f()
	{
	    return Methods::Base_Virtual;
	}

	Methods  non_virtual_f()
	{
	    return Methods::Base_Non_Virtual;
	}

    };

    class Derived:public Base
    {
	public:
	virtual Methods  virtual_f() override
	{
	    return Methods::Derived_Virtual;
	}

	Methods  non_virtual_f()
	{
	    return Methods::Derived_Non_Virtual;
	}

    };



    TEST(Virtual_Non_Virtual,virtual_base_will_call_Base)
    {

	std::shared_ptr<Base> b=std::make_shared<Base>();

	EXPECT_THAT(b->virtual_f(),Eq(Methods::Base_Virtual));
    }

    TEST(Virtual_Non_Virtual,virtual_derived_will_call_derived)
    {

	std::shared_ptr<Base> b=std::make_shared<Derived>();

	EXPECT_THAT(b->virtual_f(),Eq(Methods::Derived_Virtual));
    }


    TEST(Virtual_Non_Virtual,non_virtual_base_will_call_base)
    {

	std::shared_ptr<Base> b=std::make_shared<Base>();

	EXPECT_THAT(b->non_virtual_f(),Eq(Methods::Base_Non_Virtual));
    }

    TEST(Virtual_Non_Virtual,non_virtual_derived_will_call_base)
    {

	std::shared_ptr<Base> b=std::make_shared<Derived>();

	EXPECT_THAT(b->non_virtual_f(),Eq(Methods::Base_Non_Virtual));
    }


}






#include "gmock/gmock.h"
#include <memory>


using namespace testing;


namespace gmock_test
{

    class Base
    {
        public:
        virtual void func()=0;
        virtual void do_this(int th){};
        virtual int do_that(int th) { return 0; }
    };

    class Mock_Derived:public Base
    {
        public:
        MOCK_METHOD(void,func,(),(override));
        MOCK_METHOD(int,do_that,(int th),(override));
        MOCK_METHOD(void,do_this,(int),(override));

    };
}



TEST(gmock_test,test_call)
{
    std::shared_ptr<gmock_test::Mock_Derived> mock=std::make_shared<gmock_test::Mock_Derived>();
    std::shared_ptr<gmock_test::Base> base= mock;
    EXPECT_CALL(*(mock.get()),func())
    .Times(1);
    base->func();
}


TEST(gmock_test,test_call_strict)
{
    std::shared_ptr<StrictMock<gmock_test::Mock_Derived>> mock = std::make_shared<StrictMock<gmock_test::Mock_Derived>>();
    std::shared_ptr<gmock_test::Base> base= mock;
    EXPECT_CALL(*(mock.get()),func());
    base->func();
}

TEST(gmock_test,test_do_that)
{
    std::shared_ptr<gmock_test::Mock_Derived> mock = std::make_shared<gmock_test::Mock_Derived>();
    std::shared_ptr<gmock_test::Base> base= mock;
    ON_CALL(*mock,do_that).WillByDefault(Return(1));


    EXPECT_CALL(*mock,do_that(0))
    .Times(1)
    .WillOnce(Return(3));

    EXPECT_THAT(base->do_that(0),Eq(3));
}

TEST(gmock_test,test_sequence)
{
    InSequence usSeq;//make sequence matter 

    std::shared_ptr<gmock_test::Mock_Derived> mock=std::make_shared<gmock_test::Mock_Derived>();
    std::shared_ptr<gmock_test::Base> base= mock;
    EXPECT_CALL(*mock,func);
    EXPECT_CALL(*mock,do_this);

    base->func();
    base->do_this(0);
}
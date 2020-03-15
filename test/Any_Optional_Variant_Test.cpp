


#include "gmock/gmock.h"

#include <any>
#include <typeinfo>

using namespace testing;

namespace cxx_any
{
    struct A
    {
        int b=-1;

    };
}



TEST(CXX_any,test_any_convert_back)
{
    std::any a1=42;
    EXPECT_THAT(std::any_cast<int>(a1),Eq(42));
}

TEST(CXX_any,test_any_assign_to_int)
{
    //can be use as any value
    std::any a=10;
    EXPECT_THAT(a.type().name(),Eq(typeid(int).name()));
}

TEST(CXX_any,test_any_assign_to_float)
{
    std::any a = 10.f;
    EXPECT_THAT(a.type().name(),Eq(typeid(float).name()));
}

TEST(CXX_any,test_any_assign_to_bool)
{
    std::any a = true;
    EXPECT_THAT(a.type().name(),Eq(typeid(bool).name()));
}

TEST(CXX_any,test_any_assign_to_my_struct)
{
    std::any a = cxx_any::A();
    EXPECT_THAT(a.type().name(),Eq(typeid(cxx_any::A).name()));
}

TEST(CXX_any, test_any_cast_succeed){
    EXPECT_NO_THROW(
        {
            std::any a = cxx_any::A();
            auto p = std::any_cast<cxx_any::A>(a);
        });
}

TEST(CXX_any, test_any_cast_failed)
{
    EXPECT_THROW(
        {
            std::any a = cxx_any::A();
            auto p = std::any_cast<int>(a);
        },
        std::bad_any_cast
    );
}

#include "gmock/gmock.h"

#include <any>
#include <optional>
#include <typeinfo>

using namespace testing;

/////////optional///////////////


namespace cxx_optional
{
    std::optional<int> my_func(std::optional<int> a )
    {
        if(a)
        {
            return a.value()+1;
        }
        else
        {
            return {};
        }

    }
}


TEST(CXX_Optional, test_aftar_assign_has_value)
{
    std::optional<int> a;
    a=12;
    EXPECT_THAT(a.has_value(), Eq(true));
}

TEST(CXX_Optional,test_dereferencing)
{
    std::optional<int> a;
    a=12;
    EXPECT_THAT(*a, Eq(12));
}

TEST(CXX_Optional,test_value_or_without_value)
{
    std::optional<int> a;
    EXPECT_THAT(a.value_or(13), Eq(13));
}

TEST(CXX_Optional,test_value_or_with_value)
{
    std::optional<int> a;
    a=12;
    EXPECT_THAT(a.value_or(13), Eq(12));
}

//use in delay construction
TEST(CXX_Optional,test_emplace_one)
{
    std::optional<int> a;
    a=12;
    a.emplace(13);
    EXPECT_THAT(*a, Eq(13));
}
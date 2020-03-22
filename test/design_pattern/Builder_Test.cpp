


#include "gmock/gmock.h"
#include <string>

namespace builder_test
{

    class Data
    {
    public:
        std::string name;
        int age;
    };

    class Data_Builder
    {
    private:
        Data data{};

    public:
        Data_Builder &set_name(const std::string &name)
        {
            data.name = name;
            return *this;
        }

        Data_Builder &set_age(int age)
        {
            data.age = age;
            return *this;
        }

        Data build()
        {
            return data;
        }
    };
} // namespace builder_test

using namespace testing;

TEST(Builder_Test,test_build_name_then_age)
{
    builder_test::Data_Builder builder;
    builder_test::Data data = builder.set_name("aa")
                                 .set_age(12)
                                 .build();

    EXPECT_THAT(data.age, Eq(12));
}

TEST(Builder_Test,test_build_age_then_name_)
{
    builder_test::Data_Builder builder;
    builder_test::Data data = builder.set_age(12)
                                  .set_name("aa")
                                  .build();

    EXPECT_THAT(data.age, Eq(12));
}
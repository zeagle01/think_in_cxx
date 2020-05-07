
#include "gmock/gmock.h"




namespace variadic_template
{
    template<typename... Ts>
    void ignore(Ts... ts)
    {

    }



    template<typename T>
    T sum(T a)
    {
        return  a;
    }

    template<typename T ,typename... Ts>
    T sum(T a,Ts... ts)
    {
        return a + sum(ts...);
    }

}



using namespace testing;
using namespace variadic_template;

TEST(varidadic_tempalte,calling)
{
    variadic_template::ignore(1.0,2,"aaa");
}

TEST(varidadic_tempalte,add)
{
    auto sum0=variadic_template::sum(1.0,1.,1.0);
    EXPECT_THAT(sum0,Eq(3));

    auto sum1=variadic_template::sum(1.0,1.,1.0,1.0);

    EXPECT_THAT(sum1,Eq(4));
}
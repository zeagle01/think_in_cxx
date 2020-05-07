



#include "gmock/gmock.h"
#include <set>



using namespace testing;


namespace std_set
{
    struct A{

        int i;
        float f;
    };

    struct Order_By_Float
    {
        bool operator()(const A& l,const A& r) const 
        {
            return l.f < r.f;
        }

	};

}


TEST(Std_Set,test_insert_multiple_key)
{
    std::set<int> s;
    s.insert(1);
    s.insert(1);
    s.insert(2);


    EXPECT_THAT(s,ElementsAre(1,2));

}


TEST(Std_Set,test_insert_multiple_key_A)
{
    std::set<std_set::A,std_set::Order_By_Float> s;
    s.insert(std_set::A{0,1.0});
    s.insert(std_set::A{0,1.0});
    s.insert(std_set::A{0,1.1});

    EXPECT_THAT(s.size(),Eq(2));

}
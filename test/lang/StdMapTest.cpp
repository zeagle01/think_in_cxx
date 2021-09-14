


#include "gmock/gmock.h"
#include <map>
#include <vector>



using namespace testing;


namespace std_map
{

    struct A
    {
        int i;
        float f;
    };

    template <typename less>
    std::vector<int> fill_map_and_get_order(std::map<int, std_map::A, less> &m, const std::vector<int> &key)
    {
        for (size_t i = 0; i < key.size(); i++)
        {
            m[key[i]] = A();
        }
        std::vector<int> map_order;
        for (auto it : m)
        {
            map_order.push_back(it.first);
        }
        return map_order;
    }

    struct greater_than
    {
        bool operator()(const int& a,const int& b) const
        {
            return a > b;
        }
    };


}


TEST(Std_Map,test_default_order)
{
    std::map<int, std_map::A> m;
    auto map_order = fill_map_and_get_order(m, {0, 1, 2});
    EXPECT_THAT(map_order,ElementsAre(0,1,2));
}

TEST(Std_Map,test_default_order_random)
{
    std::map<int, std_map::A> m;
    auto map_order = fill_map_and_get_order(m, {2, 0, 1});
    EXPECT_THAT(map_order,ElementsAre(0,1,2));
}


TEST(Std_Map,test_greater_compare)
{
    std::map<int, std_map::A, std_map::greater_than> m;
    auto map_order = fill_map_and_get_order(m, {2, 0, 1});
    EXPECT_THAT(map_order,ElementsAre(2,1,0));
}


namespace with_member_less_map
{

    struct With_Member_Less
    {
        float f;
        int i;

        //const here is important
		bool operator<(const With_Member_Less& o) const
        {
            return i < o.i;
        }

		//friend bool operator<(const With_Member_Less& left,const With_Member_Less& right)
        //{
        //    return left.i < right.i;
        //}

    };


	TEST(With_Member_Less_Test, test_order)
	{
		std::map<With_Member_Less,int > m;

        auto a = With_Member_Less{ 1.0f,1 };
		m[a] = 0;
		m[With_Member_Less{ 1.0f,0 }] = 1;


		std::vector<int> exp{ 0,1 };
        std::vector<int> act;
        for (const auto& it : m)
        {
            act.push_back(it.first.i);
        }

        for (int i = 0; i < exp.size(); i++)
        {
            EXPECT_THAT(act[i], Eq(exp[i]));
        }

	}
}




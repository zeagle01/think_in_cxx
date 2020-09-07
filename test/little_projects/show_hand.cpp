


#include "gmock/gmock.h" 
#include <vector>

using namespace testing;



namespace show_hand
{

    struct Hand
    {
        std::vector<char> h;

    };


    bool operator<(const Hand& l,const Hand& r)
    { 
        return true;
    }

    TEST(Show_Hand, flush_streight_greater)
    {
        Hand h0{{'7','8','9','t','j'}};
        Hand h1{{'7','8','9','t','k'}};

        EXPECT_THAT(h1,Lt(h1));
    }
}
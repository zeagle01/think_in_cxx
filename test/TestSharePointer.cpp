

#include "gtest/gtest.h"
#include <memory>

class Data
{
    int32_t *d;
    int32_t my_id;

  public:
    static int32_t id;
    int32_t get_id(){
        return my_id;
    }
    Data()
    {
        d = new int32_t[10];
        my_id = id;
        printf("in Data%d's constructor\n", my_id);
        id++;
    }
    ~Data()
    {
        delete[] d;
        printf("in Data%d's destructor\n", my_id);
    }
};
int32_t Data::id = 0;


TEST(TestSharePointer, use_data)
{
    std::shared_ptr<Data> p;
    {
        std::shared_ptr<Data> p0=std::make_shared<Data>();
        std::shared_ptr<Data> p1=p0;
        EXPECT_EQ(p1->get_id(),p0->get_id());
        p0=std::make_shared<Data>();
        EXPECT_NE(p1->get_id(),p0->get_id());
        std::shared_ptr<Data> p2=std::make_shared<Data>();
        p=p0;
    }
}

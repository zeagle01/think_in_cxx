


#include "gmock/gmock.h"

#include <memory>

namespace uinique_pointer_test
{

class A
{
public:
    int a = -1;

public:
    virtual void f() {};
    ~A()
    {
        printf("A destory!\n");
    }
};

std::unique_ptr<A> create_A()
{
	return std::make_unique<A>();
}

} // namespace uinique_pointer_test

using namespace  testing;


class MockA:public uinique_pointer_test::A
{
public:
    MOCK_METHOD0(f,void());
};


TEST(Unique_Pointer_Test,test_create)
{
    auto aa = std::unique_ptr<uinique_pointer_test::A>{nullptr};
    aa.reset(new uinique_pointer_test::A);
    EXPECT_THAT(aa->a, Eq(-1));
}



TEST(Unique_Pointer_Test,test_ownnership_transfer)
{
    auto p0 = std::unique_ptr<uinique_pointer_test::A>{new uinique_pointer_test::A};
    auto p1 (p0.release());
    EXPECT_THAT(p1->a, Eq(-1));
}

TEST(Unique_Pointer_Test,test_costumed_deleter)
{
    auto deleter=[&](uinique_pointer_test::A* a)
    { 
        a->f();
        delete a;
    };
    auto mockA = new MockA;

    EXPECT_CALL(*mockA,f());
    auto p0 = std::unique_ptr<uinique_pointer_test::A,decltype(deleter)>(mockA,deleter);
    //p0.reset(mockA);
}


TEST(Unique_Pointer_Test, test_function_return_unique_pointer)
{
	auto a=uinique_pointer_test::create_A();
	EXPECT_THAT(a->a, Eq(-1));

}






#include "gmock/gmock.h"


using namespace testing;



namespace visitor_test
{

class Visitor;
class Shape
{
public:
    virtual void accept(Visitor &visitor) = 0;
};

class Circle : public Shape
{
public:
    void Circle::accept(Visitor &visitor) override;
};

class Visitor
{
public:
    virtual void visit(Circle &sh) = 0;
};



///////////////concrete shape/////////////////////
void Circle::accept(Visitor &visitor)
{
    visitor.visit(*this);
}
///////////////concrete shape/////////////////////



///////////////concrete visitor/////////////////////

class Print_Name_Visitor: public Visitor
{
    public:
    virtual void visit(Circle& sh) override
    {
        printf("this is circle \n");
    }
};


class Visitor_Mock: public Visitor
{
    public:
    MOCK_METHOD(void,visit,(Circle& ),(override));

    void delegate_to_fake()
    {
        ON_CALL(*this, visit).WillByDefault([this](Circle &sh) {
            return fake.visit(sh);
        });
    }

    private:
    Print_Name_Visitor fake;
};

///////////////concrete visitor/////////////////////




} // namespace visitor_test

TEST(Visitor_Test,test_use)
{
    //InSequence forceExpectationOrder;
		
    visitor_test::Visitor_Mock a_visitor;
    a_visitor.delegate_to_fake();

    visitor_test::Circle circle;
	EXPECT_CALL(a_visitor, visit(Ref(circle)));

    circle.accept(a_visitor);
}




#include "gmock/gmock.h"

#include <string>

#include <memory>

using namespace testing;



namespace visitor_test
{


    ///cyclic visitor
    //

    class Visitor;
    class Shape;

    struct Shape
    {
	virtual void accept(Visitor*) {}
    };

    struct Square:public Shape
    {
	virtual void accept(Visitor* visitor) override;
    };

    struct Triangle:public Shape
    {
	virtual void accept(Visitor* visitor) override;
    };

    struct Rectangle:public Square
    {
	virtual void accept(Visitor* visitor) override;
    };



    struct Visitor
    {
	virtual void visit(Square* s){}
	virtual void visit(Triangle* s){}
	virtual void visit(Rectangle* s){}
    };

    struct Printer :public Visitor
    {
	virtual void visit(Square* s) override
	{
	    m_print_string="square";
	}

	virtual void visit(Triangle* s) override
	{
	    m_print_string="triangle";
	}

	virtual void visit(Rectangle* s) override
	{
	    m_print_string="rectangle";
	}

	std::string get_print_string(){return m_print_string;}
	std::string m_print_string;
    };


    //////////implement here ,because cyclic reference
    void Square::accept(Visitor* visitor)  
    { 
	visitor->visit(this);
    }

    void Triangle::accept(Visitor* visitor)  
    { 
	visitor->visit(this);
    }

    void Rectangle::accept(Visitor* visitor)  
    { 
	visitor->visit(this);
    }




    ///test
    //
    //



    class Printer_Test:public Test  
    {
	public:

	    Printer_Test()
	    {

		visitor=std::make_unique<Printer>();
	    }

	protected:

	std::unique_ptr<Printer> visitor;

    };


    TEST_F(Printer_Test,square_print_square)
    {

	std::unique_ptr<Shape> e=std::make_unique<Square>();


	e->accept(visitor.get());

	EXPECT_THAT(visitor->get_print_string(),Eq("square"));


    }

    TEST_F(Printer_Test,triangle_print_triangle)
    {

	std::unique_ptr<Shape> e=std::make_unique<Triangle>();


	e->accept(visitor.get());

	EXPECT_THAT(visitor->get_print_string(),Eq("triangle"));

    }

    TEST_F(Printer_Test,rectangle_print_rectangle)
    {

	std::unique_ptr<Shape> e=std::make_unique<Rectangle>();


	e->accept(visitor.get());

	EXPECT_THAT(visitor->get_print_string(),Eq("rectangle"));

    }



} // namespace visitor_test


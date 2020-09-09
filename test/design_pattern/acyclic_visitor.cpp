



#include "gmock/gmock.h"

#include <string>

#include <memory>

#include <initializer_list>
using namespace testing;



namespace acyclic_visitor_test
{


    //

	struct VisitorBase
	{
		virtual ~VisitorBase() = default;
	};

    template<typename Visitable>
    struct Visitor
    {
		virtual void visit(Visitable* s) = 0;
    };

    struct Shape
    {
		virtual void accept(VisitorBase* v)
		{
			using ShapeVisitorT = Visitor<Shape>;

			if (auto visitor = dynamic_cast<ShapeVisitorT*>(v))
			{
				visitor->visit(this);
			}

		}
    };

    struct Square:public Shape
    {
//	virtual void accept(Visitor* visitor) override;
    };

    struct Triangle:public Shape
    {
//	virtual void accept(Visitor* visitor) override;
    };

    struct Rectangle:public Square
    {
//	virtual void accept(Visitor* visitor) override;
    };

    struct Shape_Collection:public Shape
    {
		Shape_Collection(std::initializer_list<std::unique_ptr<Shape>> shapes)
		{

			for (auto &s : shapes)
			{
				auto e = std::move(const_cast<std::unique_ptr<Shape>&>(s)); //this one is tricky ,initializer_list unique_ptr element must remove "constness" before it move
				m_shapes.emplace_back(std::move(e));
			}
		}
//	virtual void accept(Visitor* visitor) override;

	std::vector<std::unique_ptr<Shape>> m_shapes;
    };




	struct Printer :
		VisitorBase,
		Visitor<Square>,
		Visitor<Triangle>,
		Visitor<Rectangle>
	{
		virtual void visit(Square* s) override
		{
			m_print_string = "square";
		}

		virtual void visit(Triangle* s) override
		{
			m_print_string = "triangle";
		}

		virtual void visit(Rectangle* s) override
		{
			m_print_string = "rectangle";
		}

		//	virtual void visit(Shape_Collection* s) override
		//	{
		//	    std::string collection_string="";
		//	    for(auto& sit:s->m_shapes)
		//	    {
		//		sit->accept(this);
		//		collection_string+=m_print_string+" "; //this one is interisting!
		//	    }
		//	    m_print_string=collection_string;
		//	}

		std::string get_print_string() { return m_print_string; }
		std::string m_print_string;
	};


    //////////implement here ,because cyclic reference
//    void Square::accept(Visitor* visitor)  
//    { 
//	visitor->visit(this);
//    }
//
//    void Triangle::accept(Visitor* visitor)  
//    { 
//	visitor->visit(this);
//    }
//
//    void Rectangle::accept(Visitor* visitor)  
//    { 
//	visitor->visit(this);
//    }
//
//    void Shape_Collection::accept(Visitor* visitor)  
//    { 
//	visitor->visit(this);
//    }




    ///test
    //
    //



    class Acyclic_Printer_Test:public Test  
    {
	public:

	    Acyclic_Printer_Test()
	    {

		visitor=std::make_unique<Printer>();
	    }

	protected:

	std::unique_ptr<Printer> visitor;

    };


	TEST_F(Acyclic_Printer_Test, square_print_square)
	{

		std::unique_ptr<Shape> e = std::make_unique<Square>();


		e->accept(visitor.get());

		EXPECT_THAT(visitor->get_print_string(), Eq("square"));


	}

	TEST_F(Acyclic_Printer_Test, triangle_print_triangle)
	{

		std::unique_ptr<Shape> e = std::make_unique<Triangle>();


		e->accept(visitor.get());

		EXPECT_THAT(visitor->get_print_string(), Eq("triangle"));

	}

	TEST_F(Acyclic_Printer_Test, rectangle_print_rectangle)
	{

		std::unique_ptr<Shape> e = std::make_unique<Rectangle>();


		e->accept(visitor.get());

		EXPECT_THAT(visitor->get_print_string(), Eq("rectangle"));

	}

//	TEST_F(Printer_Test, shape_collection_print_shapes_in_that_collection)
//	{
//		//std::unique_ptr<Shape> e=std::make_unique<Shape_Collection>({std::make_unique<Triangle>(),std::make_unique<Triangle>(),std::make_unique<Triangle>()});
//		//std::unique_ptr<Shape_Collection> e=std::make_unique<Shape_Collection>({std::make_unique<Triangle>(),std::make_unique<Triangle>(),std::make_unique<Triangle>()});
//		//
//		std::unique_ptr<Shape_Collection> e;
//
//		e.reset(
//			new Shape_Collection(
//				{ std::make_unique<Triangle>(),std::make_unique<Square>(),std::make_unique<Rectangle>() }
//			)
//		);
//
//		e->accept(visitor.get());
//
//		EXPECT_THAT(visitor->get_print_string(), Eq("triangle square rectangle "));
//
//	}
}

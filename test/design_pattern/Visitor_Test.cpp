


#include "gmock/gmock.h"

#include <string>

#include <memory>

#include <initializer_list>
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

	struct Square :public Shape
	{
		virtual void accept(Visitor* visitor) override;
	};

	struct Triangle :public Shape
	{
		virtual void accept(Visitor* visitor) override;
	};

	struct Rectangle :public Square
	{
		virtual void accept(Visitor* visitor) override;
	};

	struct Shape_Collection :public Shape
	{
		Shape_Collection(std::initializer_list<std::unique_ptr<Shape>> shapes)
		{

			for (auto& s : shapes)
			{
				auto e = std::move(const_cast<std::unique_ptr<Shape>&>(s)); //this one is tricky ,initializer_list unique_ptr element must remove "constness" before it move
				m_shapes.emplace_back(std::move(e));
			}
		}
		virtual void accept(Visitor* visitor) override;

		std::vector<std::unique_ptr<Shape>> m_shapes;
	};



	struct Visitor
	{
		virtual void visit(Square* s) {}
		virtual void visit(Triangle* s) {}
		virtual void visit(Rectangle* s) {}
		virtual void visit(Shape_Collection* s) {}
	};

	struct Printer :public Visitor
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

		virtual void visit(Shape_Collection* s) override
		{
			std::string collection_string = "";
			for (auto& sit : s->m_shapes)
			{
				sit->accept(this);
				collection_string += m_print_string + " "; //this one is interisting!
			}
			m_print_string = collection_string;
		}

		std::string get_print_string() { return m_print_string; }
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

	void Shape_Collection::accept(Visitor* visitor)
	{
		visitor->visit(this);
	}




	///test
	//
	//



	template<typename T>
	struct Param_Record
	{
		std::shared_ptr<T> visitor;

		std::vector<std::string> print_string;
	};


	class Printer_Test :public Test, public WithParamInterface<Param_Record<Printer>>
	{
	public:


		Printer_Test()
		{

			visitor = std::make_unique<Printer>();


			shapes.push_back(std::make_unique<Square>());
			shapes.push_back(std::make_unique<Triangle>());
			shapes.push_back(std::make_unique<Rectangle>());
			std::unique_ptr<Shape_Collection> e;
			e.reset(
				new Shape_Collection(
					{ std::make_unique<Triangle>(),std::make_unique<Square>(),std::make_unique<Rectangle>() }
				)
			);
			shapes.push_back(std::move(e));
		}

	protected:

		std::unique_ptr<Printer> visitor;

		std::vector<std::unique_ptr<Shape>> shapes;

	};



	TEST_P(Printer_Test, test_all)
	{
		auto param = GetParam();

		std::vector<std::string > act;
		for (auto& shape : shapes)
		{
			shape->accept(param.visitor.get());
			act.push_back(param.visitor->get_print_string());
		}
		EXPECT_THAT(act, Eq(param.print_string));

	}

	INSTANTIATE_TEST_SUITE_P(Printer_Test_All0, Printer_Test,
		Values(
			Param_Record<Printer>{ std::make_shared<Printer>(),std::vector<std::string>{"square","triangle","rectangle","triangle square rectangle "} }
		)
		);














	template<typename returnT>
	struct Visitor_Adapter :public Visitor
	{
		template<typename T>
		using Fn = std::function<returnT(Visitor_Adapter<returnT>*, T*)>;

		Visitor_Adapter(
			Fn<Triangle> triangle_f,
			Fn<Square> square_f,
			Fn<Rectangle> rectangle_f,
			Fn<Shape_Collection> collection_f
		) :
			m_triangle_f(triangle_f),
			m_square_f(square_f),
			m_rectangle_f(rectangle_f),
			m_shape_collection_f(collection_f)
		{}


		returnT get_result()
		{
			return m_result;
		}

		virtual void visit(Square* s) override
		{
			m_result = m_square_f(this, s);
		}

		virtual void visit(Triangle* s) override
		{
			m_result = m_triangle_f(this, s);
		}

		virtual void visit(Rectangle* s) override
		{
			m_result = m_rectangle_f(this, s);
		}

		virtual void visit(Shape_Collection* s) override
		{
			m_result = m_shape_collection_f(this, s);
		}

		Fn<Triangle> m_triangle_f;
		Fn<Square> m_square_f;
		Fn<Rectangle> m_rectangle_f;
		Fn<Shape_Collection> m_shape_collection_f;


		returnT m_result;

	};




	TEST_F(Printer_Test, test_adapter)
	{
		using Printer_Adapter = Visitor_Adapter<std::string>;
		std::shared_ptr<Printer_Adapter> print_adaptor_visitor = std::make_shared<Printer_Adapter>(
			[](auto* ,auto* )
			{
				return "triangle";

			}
			,
				[](auto*,auto* )
			{
				return "square";

			}
				,
				[](auto* ,auto* )
			{
				return  "rectangle";

			}
				,
				[](auto* visitor,auto* s)
			{

				std::string collection_string = "";
				for (auto& sit : s->m_shapes)
				{
					sit->accept(visitor);
					collection_string += visitor->get_result() + " "; //this one is interisting!
				}
				return collection_string;
			}

			);



		std::vector<std::string> exp = { "square","triangle","rectangle","triangle square rectangle " };
		
		std::vector<std::string> act;
		for (auto& shape : shapes)
		{
			shape->accept(print_adaptor_visitor.get());
			act.push_back(print_adaptor_visitor->get_result());
		}
		EXPECT_THAT(act, Eq(exp));

	}

} // namespace visitor_test


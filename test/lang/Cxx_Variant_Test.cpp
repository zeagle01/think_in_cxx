


#include "gmock/gmock.h"


#include <variant>
#include <vector>

using namespace testing;

namespace cxx_variant
{

	class A
	{
	public:
		std::string do_something()
		{
			return "A";
		}

		std::string get_A()
		{
			return do_something();
		}
	};

	class B
	{
	public:
		std::string do_something()
		{
			return "B";
		}

		std::string get_B()
		{
			return do_something();
		}
	};

	class C
	{
	public:
		std::string do_something()
		{
			return "C";
		}

		std::string get_C()
		{
			return do_something();
		}
	};
	using Doable = std::variant<A, B, C>;

	TEST(Cxx_Variant, test_duck)
	{
		std::vector<Doable> doables;
		doables.push_back(A());
		doables.push_back(B());

		//				won't work if use Doable as argument here
		//							||	
		//							\/
		//auto doable_visitor = [](Doable& doable) { return doable.do_something(); }; 

		auto doable_visitor = [](auto& doable) { return doable.do_something(); };

		std::vector<std::string> act;
		for (size_t i = 0; i < doables.size(); i++)
		{
			act.push_back(std::visit(doable_visitor, doables[i]));
		}

		std::vector<std::string> exp{ "A","B" };
		EXPECT_THAT(act, Eq(exp));
	}



	struct Variant_Hoder
	{
		Doable data;
	};

	TEST(Cxx_Variant, test_duck_with_pointer)
	{
		std::vector<std::shared_ptr<Variant_Hoder>> doables;
		doables.push_back(std::make_shared<Variant_Hoder>(A{}));
		doables.push_back(std::make_shared<Variant_Hoder>(B{}));

		//				won't work if use Doable as argument here
		//							||	
		//							\/
		//auto doable_visitor = [](Doable& doable) { return doable.do_something(); }; 

		auto doable_visitor = [](auto& doable) { return doable.do_something(); };

		std::vector<std::string> act;
		for (size_t i = 0; i < doables.size(); i++)
		{
			act.push_back(std::visit(doable_visitor, doables[i]->data));
		}

		std::vector<std::string> exp{ "A","B" };
		EXPECT_THAT(act, Eq(exp));
	}


	template<typename ... Functions>
	struct Overload :Functions ...
	{
		using Functions::operator()...;
		Overload(Functions... functions) :Functions(functions)... {}
	};

	TEST(Cxx_Variant, test_visit_one)
	{
		Doable da = A{};
		Doable db = B{};
		Doable dc = C{};

		auto fn = [](Doable& d) {
			return std::visit(
				Overload
				(
					[](A& a) { return a.get_A(); },
					[](B& a) { return a.get_B(); },
					[](auto& a) ->std::string { return ""; } //default case
				),
				d
			);
		};

		EXPECT_THAT(fn(da), Eq("A"));
		EXPECT_THAT(fn(db), Eq("B"));
		EXPECT_THAT(fn(dc), Eq(""));
	}


	TEST(Cxx_Variant, test_multiple_dispatch)
	{
		Doable da = A{};
		Doable db = B{};
		Doable dc = C{};

		auto fn = [](auto& d0, auto& d1)
		{
			return	std::visit(
				Overload
				(
					[](B& b, A& a) { return   b.get_B() + a.get_A(); },
					[](A& a, B& b) { return a.get_A() + b.get_B(); },
					[](auto& a, auto& b) ->std::string { return ""; } //default case
				),
				d0, d1
			);
		};

		EXPECT_THAT(fn(da, db), Eq("AB"));
		EXPECT_THAT(fn(db, da), Eq("BA"));
		EXPECT_THAT(fn(da, da), Eq(""));
		EXPECT_THAT(fn(dc, da), Eq(""));
	}


} // namespace cxx_variant

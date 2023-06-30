
#include "gmock/gmock.h"

#include "product_factory/product.h"


using namespace testing;

namespace product_factory
{

	template<typename Fn, typename Seq>
	struct loop_index;

	template<typename Fn, int ...I>
	struct loop_index<Fn, std::index_sequence<I...>>
	{
		template<typename ...P>
		static void apply(P&& ...p)
		{
			(Fn::template apply<I>(std::forward<P>(p)...),...);
		}
	};

	template<typename Fn, int N>
	struct loop_index_N
	{
		template<typename ...P>
		static void apply(P&& ...p)
		{
			loop_index<Fn, decltype(std::make_index_sequence<N>) >::apply(std::forward<P>(p)...);
		}
	};


	struct create_by_index
	{
		template<int I>
		static void apply(Product_Type type, std::unique_ptr<Product>& product)
		{
			if (int(type) == I)
			{
				constexpr Product_Type t = static_cast<Product_Type>(I);
				product = Product::create<t>();
			}

		}
	};

	static std::unique_ptr<Product> create_product(Product_Type type)
	{
		std::unique_ptr<Product> ret;
		loop_index_N<create_by_index, int(Product_Type::Count)>::apply(type, ret);
		return ret;
	}


	TEST(Product_Factory_Test, get_concrte_product)
	{
		auto product = Product::create<Product_Type::A>();

		EXPECT_THAT(product->func(), Eq("A"));

	}

	TEST(Product_Factory_Test, get_concrte_product_by_parameter)
	{
		auto product = create_product(Product_Type::B);

		EXPECT_THAT(product->func(), Eq("B"));

	}
}

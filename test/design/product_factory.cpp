
#include "gmock/gmock.h"

#include "product_factory/product.h"


using namespace testing;

namespace product_factory
{

	TEST(Product_Factory_Test, get_concrte_product)
	{
		auto product = Product::create<Product_Type::A>();

		EXPECT_THAT(product->func(), Eq("A"));

	}
}




#include "concrete_product.h"
#include <string>



class Product_A :public Product
{
	std::string func() override
	{
		return "A";
	}

};



template<> std::unique_ptr<Product> Product::create<Product_Type::A>()
{
	return std::make_unique<Product_A>();
}


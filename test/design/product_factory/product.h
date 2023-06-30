



#pragma once

#include <memory> 
#include <string>

enum class Product_Type
{
	A,
	B,
	Count
};

class Product
{
public:
	virtual std::string func() = 0;

	template<Product_Type pt> static std::unique_ptr<Product> create();
};



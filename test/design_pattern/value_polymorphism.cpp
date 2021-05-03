

#include "gmock/gmock.h"
#include <any>
#include <string>

using namespace testing;

namespace value_polymorphism
{


	template<typename Interface>
	class Implementation
	{

	public:
		template<typename ConcreteType>
		Implementation(ConcreteType obj):
			m_storage(obj),
			getter(
				[](std::any& a)-> Interface& {
					return std::any_cast<ConcreteType&>(a);
				}

			)
		{}

		Interface* operator->()
		{
			return &getter(m_storage);
		}

	private:
		std::any m_storage;
		Interface& (*getter)(std::any&);
	};





	class Base 
	{
	public:
		virtual std::string f() = 0;
	};

	class A : public Base 
	{
	public:
		std::string f() override
		{
			return "A";
		};
	};

	class B : public Base 
	{
	public:
		std::string f() override
		{
			return "B";
		};
	};


	TEST(value_polymorphism_Test, use)
	{

		Implementation<Base> base = A{};
		auto act = base->f();
		EXPECT_THAT(act, Eq("A"));
		base = B{};
		act = base->f();
		EXPECT_THAT(act, Eq("B"));

	}


}

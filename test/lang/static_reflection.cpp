


#include "gmock/gmock.h"
#include <type_traits>
#include <string>

using namespace testing;


namespace reflection_test
{


	struct anyT
	{
		size_t dummy; //for construtable

		template<typename T>
		constexpr operator T& () const noexcept;
	};



	template<typename T, std::size_t I0, std::size_t... I>
	constexpr auto detect_fileds_count(std::size_t& out, std::index_sequence<I0, I...>) -> decltype(T{ anyT{I0},anyT{I}... })
	{
		out = sizeof...(I) + 1;
		return T{};
	}

	template<typename T, std::size_t... I>
	constexpr void detect_fileds_count(std::size_t& out, std::index_sequence< I...>)
	{
		detect_fileds_count<T>(out, std::make_index_sequence<sizeof...(I) - 1>{});
	}



	template<typename T>
	constexpr size_t cout_field()
	{
		std::size_t  field_count = 0;
		detect_fileds_count<T, 0, 1, 2, 3, 4, 5,
			6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17
		>(field_count, std::make_index_sequence<18>{});
		return field_count;
	}


	template <typename T>
	constexpr auto as_tuple(T&& val) noexcept
	{

		constexpr auto count = cout_field<T>();

		if constexpr (count == 1)
		{

			auto& [a] = std::forward<T>(val);

			return std::make_tuple(a);

		}
		else if constexpr (count == 2)
		{
			auto& [a, b] = std::forward<T>(val);
			return std::make_tuple(a, b);
		}
		else if constexpr (count == 3)
		{
			auto& [a, b, c] = std::forward<T>(val);
			return std::make_tuple(a, b, c);
		}
		else if constexpr (count == 3)
		{
			auto& [a, b, c] = std::forward<T>(val);
			return std::make_tuple(a, b, c);
		}
		else if constexpr (count == 4)
		{
			auto& [a, b, c, d] = std::forward<T>(val);
			return std::make_tuple(a, b, c, d);
		}
		else if constexpr (count == 5)
		{
			auto& [a, b, c, d, e] = std::forward<T>(val);
			return std::make_tuple(a, b, c, d, e);
		}
		else if constexpr (count == 6)
		{
			auto& [a, b, c, d, e, f] = std::forward<T>(val);
			return std::make_tuple(a, b, c, d, e, f);
		}

	}

	struct B
	{
		int bb;
	};

	struct A {
		int a;
		char b;
		float c;
		//std::string d;
		//std::vector<int> d;
		B bb;

	};


	TEST(Reflection_, fileds_count)
	{

		size_t count = 0;
		//detect_fileds_count<A,0,1,2,3,4,5>(count, std::make_index_sequence<6>{});
		count = cout_field<A>();

		EXPECT_THAT(count, Eq(3));
	}

	TEST(Reflection_, as_tuple_impl111)
	{

		A a;
		a.a = 0;
		a.b = 'b';
		a.c = 2.;
		auto r = as_tuple<A>(std::move(a));

//		std::apply([](auto&&... args)
//			{((std::cout << args << std::endl), ...); }, r
//		);
	}




}
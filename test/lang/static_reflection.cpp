


#include "gmock/gmock.h"
#include <type_traits>
#include <string>

using namespace testing;


namespace reflection_test
{

	namespace _1
	{
		struct anyT
		{
			size_t dummy; //for construtable

			template <typename T>
			constexpr operator T& () const noexcept;
		};

		template <typename T, std::size_t I0, std::size_t... I>
		constexpr auto detect_fileds_count(std::size_t& out, std::index_sequence<I0, I...>) -> decltype(T{ anyT{I0}, anyT{I}... })
		{
			out = sizeof...(I) + 1;
			return T{};
		}

		template <typename T, std::size_t... I>
		constexpr void detect_fileds_count(std::size_t& out, std::index_sequence<I...>)
		{
			detect_fileds_count<T>(out, std::make_index_sequence<sizeof...(I) - 1>{});
		}

		template <typename T, typename U = void, std::size_t... I>
		struct detect_fileds_count1
		{
			static constexpr size_t value = sizeof...(I) - 1;
		};

		template <typename T, std::size_t... I>
		struct detect_fileds_count1 < T, std::void_t<decltype(T{ anyT{I}... }) > , I... >
		{
			static constexpr size_t value = detect_fileds_count1<T, void, sizeof...(I), I...>::value;
		};

		template <typename T>
		constexpr size_t cout_field()
		{
			std::size_t field_count = 0;
			return detect_fileds_count1<T>::value;
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

		struct A
		{
			int a;
			char b;
			float c;
			//std::string d;
			//std::vector<int> d;

			//B bb;
		};

		TEST(Reflection_, fileds_count)
		{

			size_t count = 0;
			count = cout_field<A>();

			EXPECT_THAT(count, Eq(3));
		}

		TEST(Reflection_, fileds_count_of_int)
		{

			auto count = cout_field<int>();

			EXPECT_THAT(count, Eq(1));
		}

		TEST(Reflection_, as_tuple_impl111)
		{

			A a;
			a.a = 0;
			a.b = 'b';
			a.c = 2.;
			auto r = as_tuple<A>(std::move(a));

			std::apply([](auto&&... args) { ((
				std::cout << args << std::endl),
				...); }, r);
		}

	} // namespace _1

	namespace _2
	{

		struct anyT
		{
			size_t dummy;
			template <typename T>
			operator T& ();

			template <typename T>
			operator T* () { return nullptr; }
		};

		template <typename T, typename I0 = void, size_t... I>
		struct field_count
		{
			static constexpr size_t value = sizeof...(I) - 1;
		};

		template <typename T, size_t... I>
		struct field_count < T, std::void_t< decltype(T{ anyT{I}... }) > , I... >
		{
			static constexpr size_t value = field_count<T, void, sizeof...(I), I... >::value;
		};

		template <typename T>
		constexpr auto as_tuple(T&& val) noexcept
		{

			constexpr auto count = field_count<T>::value;

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


		struct A_with_P
		{
			int* a;
			float* b;

		};

		struct A_pod
		{
			int a;
			float b;

		};

		struct A_Mixed
		{
			int a;
			float b;
			double* c;

		};

		TEST(filed_count, int_count)
		{
			auto act = field_count<int>::value;
			EXPECT_THAT(act, Eq(1));
		}

		TEST(filed_count, pod)
		{
			auto act = field_count<A_pod>::value;
			EXPECT_THAT(act, Eq(2));
		}

		TEST(filed_count, with_pointers)
		{
			auto act = field_count<A_with_P>::value;
			EXPECT_THAT(act, Eq(2));
		}


		TEST(filed_count, mixed_with_pointer)
		{
			auto act = field_count<A_Mixed>::value;
			EXPECT_THAT(act, Eq(3));
		}

		TEST(filed_count, mixed_with_pointer_as_tuple)
		{
			A_Mixed a;
			auto act = as_tuple(std::move(a));
		}

	}

} // namespace reflection_test

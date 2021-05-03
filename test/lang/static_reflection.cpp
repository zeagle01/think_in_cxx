


#include "gmock/gmock.h"
#include <type_traits>
#include <string>

using namespace testing;


namespace reflection_test
{


	struct to_any
	{
		template<typename T>
		operator T();

		template<typename T>
		operator T*();

	};


	template<typename T, typename U = void, typename ...P>
	struct get_field_count
	{
		static constexpr int value = sizeof...(P) - 1;
	};

	template<typename T,typename ...P>
	struct get_field_count < T, std::void_t < decltype(T{ P{}... }) > , P... >
	{
		static constexpr int value = get_field_count<T, void, to_any, P...>::value;
	};




		//aggregated constructible
		struct A
		{
		};

		struct B
		{
			int a;

		};

		struct C
		{
			int a;
			float* b;
		};

		struct D
		{
			int a;
			float* b;
			float c;
		};
		
		struct E
		{
			int a;
			float* b;
			float c;
			int d;
		};


		TEST(Static_Reflection_Test, get_field_count_test)
		{
			auto act = get_field_count<A>::value;
			EXPECT_THAT(act, Eq(0));

			act = get_field_count<B>::value;
			EXPECT_THAT(act, Eq(1));

			act = get_field_count<C>::value;
			EXPECT_THAT(act, Eq(2));

			act = get_field_count<D>::value;
			EXPECT_THAT(act, Eq(3));

			act = get_field_count<E>::value;
			EXPECT_THAT(act, Eq(4));
		}
		


} // namespace reflection_test

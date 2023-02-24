

#include "gmock/gmock.h"

using namespace testing;

namespace static_loop 
{
	template<int b, int e>
	struct static_loop
	{
		template<typename Fn>
		void operator()(Fn& fn)
		{
			if (b < e)
			{
				fn(b);
				static_loop<b + 1, e>()(fn);
			}
		}
	};

	template<int N>
	struct static_loop<N, N>
	{
		template<typename Fn>
		void operator()(Fn& fn)
		{
		}
	};

	enum class Et
	{
		None,
		A,
		B,
		C,
		Size

	};

	//// won't work
	//TEST(Static_Loop, loop_a_enum)
	//{
	//	Et et = Et::C; //runtime value
	//	bool hit = false;
	//	static_loop<int(Et::None), int(Et::Size)>()(
	//			[&](int i) {
	//				if (et == Et(i))
	//				{
	//					hit = true;
	//				}
	//			}
	//		);
	//	EXPECT_TRUE(hit);
	//}



	namespace _1
	{
		template< int ... N >
		struct Sum
		{
			static constexpr inline int value = (0 + ... + N);
		};


		TEST(folding_expr, factorial)
		{
			auto act = Sum<1, 2, 3>::value;
			EXPECT_THAT(act, Eq(6));
		}
	}

	namespace _2
	{

		template<typename... P>
		struct TypeList {};

		template<typename L0, typename L1>
		struct merge;

		template< typename ...P0, typename ...P1 >
		struct merge<TypeList<P0...>, TypeList<P1...>>
		{
			using type = TypeList<P0..., P1...>;
		};


		template<
			typename ...P0, typename ...P1 ,
			template<typename ...> typename L0,
			template<typename ...> typename L1
		>
		static typename merge<L0<P0...>, L1<P1...>>::type operator+(L0<P0...>, L1<P1...>)
		{
			return {};
		}


		template<typename ...L>
		struct merge_more
		{
			using type = decltype((... + L()));
		};


		TEST(mergeList, merge_two)
		{
			using L0 = TypeList<int>;
			using L1 = TypeList<float,double>;

			using M = merge<L0, L1>::type;

			std::string name = typeid(decltype(std::declval<M>())).name();

			printf("%s \n", name.c_str());

		}

		TEST(mergeList, merge_more)
		{
			using L0 = TypeList<int>;
			using L1 = TypeList<float,double>;
			using L2 = TypeList<bool>;

			using M = merge_more<L0, L1, L2>::type;

			std::string name = typeid(decltype(std::declval<M>())).name();

			printf("%s \n", name.c_str());


		}
	}


}


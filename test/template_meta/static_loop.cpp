

#include "gtest/gtest.h"

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

	TEST(Static_Loop, loop_a_enum)
	{
		Et et = Et::C; //runtime value

		bool hit = false;
		static_loop<int(Et::None), int(Et::Size)>()(
				[&](int i) {
					if (et == Et(i))
					{
						hit = true;
					}
				}
			);

		EXPECT_TRUE(hit);

	}

}


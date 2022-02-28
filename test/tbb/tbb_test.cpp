
#include "gmock/gmock.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"

using namespace testing;


namespace tbb_test
{

	TEST(TBB_Test, test_parallel_for)
	{
		int n = 10;
		std::vector<int > a(n,1);
		std::vector<int > b(n,1);
		std::vector<int > act(n);


		tbb::parallel_for(
			tbb::blocked_range<int>(0, n),
			[&a, &b, &act](const tbb::blocked_range<int>& r) {
				for (auto i = r.begin(); i != r.end(); i++)
				{
					act[i] = a[i] + b[i];
				}
			}
		);

		std::vector<int > exp(n,2);

		EXPECT_THAT(act, Eq(exp));

	}

}

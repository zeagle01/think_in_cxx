
#include "gmock/gmock.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
#include "tbb/task_group.h"

using namespace testing;


namespace tbb_test
{

	TEST(TBB_Test, basic_test_parallel_for_with_range)
	{
		int n = 10;
		std::vector<int > a(n,1);
		std::vector<int > b(n,1);
		std::vector<int > act(n, 0);


		tbb::parallel_for(
			tbb::blocked_range(0, n),
			[&a, &b, &act](const auto& r) {
				for (auto i = r.begin(); i != r.end(); i++)
				{
					act[i] += a[i] + b[i];
				}
			}
		);

		std::vector<int > exp(n,2);
		EXPECT_THAT(act, Eq(exp));
	}

	TEST(TBB_Test, basic_test_parallel_for_with_begin_end)
	{
		int n = 10;
		std::vector<int > a(n,1);
		std::vector<int > b(n,1);
		std::vector<int > act(n, 0);

		tbb::parallel_for(0, n, [&](auto i) {
				act[i] += a[i] + b[i];
			});

		std::vector<int > exp(n,2);

		EXPECT_THAT(act, Eq(exp));

	}


	TEST(TBB_Test, basic_task_group)
	{
		tbb::task_group tg;

		int n = 8;
		std::vector<int> act(n, -1);

		auto computeFn = [&act](int i) {act[i] = i; };


		for (int i = 0; i < n; i++)
		{
			tg.run(std::bind(computeFn, i));
		}
		tg.wait();

		std::vector<int> exp(n);
		for (int i = 0; i < n; i++)
		{
			exp[i] = i;
		}
		EXPECT_THAT(act, Eq(exp));

	}
}


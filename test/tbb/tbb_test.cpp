
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



	namespace quick_sort
	{


		int partition(std::vector<int>& a, int b, int e)
		{
			int i = b - 1;
			int r = e - 1;

			for (int j = b; j < e - 1; j++)
			{
				if (a[j] <= a[r])
				{
					i++;
					std::swap(a[i], a[j]);
				}
			}

			std::swap(a[i + 1], a[r]);

			return i + 1;
		}



		void quick_sort(std::vector<int>& a, int b, int e)
		{
			if (e - b > 1)
			{
				auto p = partition(a, b, e);
				quick_sort(a, b, p);
				quick_sort(a, p + 1, e);
			}
		}


		TEST(Quick_Sort_Test, partition_invariance)
		{
			std::vector<int> input{ 4,3,2,1 };

			int p = partition(input, 0, input.size());

			for (int i = 0; i <= p; i++)
			{
				EXPECT_THAT(input[i], Le(input[p]));
			}

			for (int i = p + 1; i < input.size(); i++)
			{
				EXPECT_THAT(input[i], Gt(input[p]));
			}

		}

		TEST(Quick_Sort_Test, test_sorted)
		{
			std::vector<int> act{ 4,3,2,1 };

			quick_sort(act, 0, act.size());

			std::vector<int> exp{ 1,2,3,4 };

			EXPECT_THAT(act, Eq(exp));

		}

	}




}


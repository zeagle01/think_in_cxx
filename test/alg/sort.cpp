

#include "gmock/gmock.h"

#include <vector>
#include <limits>

using namespace testing;

namespace sort
{

	namespace quick_sort
	{


		class Sorter
		{
		public:
			void sort(std::vector<int>& in)
			{
				sort_imp(in.data(), 0, in.size() - 1);
			}

		private:
			int find_split(int* d, int b, int e)
			{
				int split = b;
				for (int i = b; i <= e; i++)
				{
					if (d[i] <= d[e])
					{
						std::swap(d[i], d[split]);
						split++;
					}
					else
					{
						continue;
					}
				}

				return split - 1;
			}

			void sort_imp(int* d, int b, int e)
			{
				if (b < e)
				{
					int split = find_split(d, b, e);
					sort_imp(d, b, split - 1);
					sort_imp(d, split + 1, e);
				}
			}
		};

		void test_helper(const std::vector<int>& in, const std::vector<int>& exp)
		{
			std::vector<int> act=in;
			Sorter sorter;
			sorter.sort(act);
			EXPECT_THAT(act, Eq(exp));
		}

		TEST(Quick_Sort, recieve_vector)
		{
			test_helper({ 3,2,1 }, { 1,2,3 });
		}

		TEST(Quick_Sort, recieve_vector_2)
		{
			test_helper({ 4,2,1 ,3 }, { 1,2,3,4 });
		}

		TEST(Quick_Sort, recieve_vector_3)
		{
			test_helper({ 4,2,1, 5,3 }, { 1,2,3,4,5 });
		}

	}

	namespace merge_sort
	{

		class Merge_Sort
		{
		public:
			std::vector<int> operator()(const std::vector<int>& in)
			{
				int n = in.size();
				auto left = std::vector<int>(in.begin(), in.begin() + n / 2);
				auto right = std::vector<int>(in.begin() + n / 2, in.end());
				left.push_back(std::numeric_limits<int>::max());
				right.push_back(std::numeric_limits<int>::max());

				std::vector<int> ret(n);
				int li = 0, ri = 0;
				for (int i = 0; i < n; i++)
				{
					if (left[li] <= right[ri])
					{
						ret[i] = left[li];
						li++;
					}
					else
					{
						ret[i] = right[ri];
						ri++;
					}

				}
				return ret;
			}

		};

		class Merge_Sort_Test:public Test
		{
		protected:
			Merge_Sort sort;

			void expect_with_input(const std::vector<int>& in, const std::vector<int>& exp)
			{
				auto act = sort(in);
				EXPECT_THAT(act, Eq(exp));
			}
		};

		TEST_F(Merge_Sort_Test, call_api)
		{
			std::vector<int> in;
			sort(in);
		}

		TEST_F(Merge_Sort_Test, size_stay_the_same)
		{
			std::vector<int> in{ 1 };
			auto act = sort(in);
			EXPECT_THAT(act.size(), Eq(in.size()));
		}

		TEST_F(Merge_Sort_Test, one_element_input_stay_the_same)
		{
			expect_with_input({ 2 }, { 2 });
		}

		TEST_F(Merge_Sort_Test, two_half_sorted_replace_into_right_place)
		{
			expect_with_input({ 4,5, 1,2 }, { 1,2,4,5 });
		}

		TEST_F(Merge_Sort_Test, two_half_sorted_with_right_place_stay_the_same)
		{
			expect_with_input({ 1,2,4,5 }, { 1,2,4,5 });
		}

		TEST_F(Merge_Sort_Test, interleved_two_sorted_half)
		{
			expect_with_input({ 1,3,2,4 }, { 1,2,3,4 });
		}


	}

}




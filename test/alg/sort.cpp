

#include "gmock/gmock.h"

#include <vector>
#include <limits>

using namespace testing;

namespace sort
{

	namespace quick_sort
	{

		class Quick_Sort
		{
		public:
			std::vector<int> operator()(const std::vector<int>& in)
			{
				std::vector<int> ret = in;
				if (!in.empty())
				{

					int split = -1;
					for (int i = 0; i<int(in.size() - 1); i++)
					{
						if (ret[i] <= ret.back())
						{
							split++;
							std::swap(ret[i], ret[split]);
						}
					}
					std::swap(ret[split + 1], ret.back());
				}
				return ret;
			}
		};

		class Quick_Sort_Test :public Test
		{
		protected:
			Quick_Sort sort;
		};

		TEST_F(Quick_Sort_Test, call_api)
		{
			std::vector<int> in{};
			std::vector<int>out = sort(in);
		}


		TEST_F(Quick_Sort_Test, place_last_one_into_right_place)
		{
			std::vector<int> in{0,1,2,	4,5,		3 };
			std::vector<int> out = sort(in);
			EXPECT_THAT(out[3], Eq(3));
		}

		TEST_F(Quick_Sort_Test, first_half_is_less_than_last_element)
		{
			std::vector<int> in{4,5,	0,1,2,	3 };
			std::vector<int> out = sort(in);
			EXPECT_THAT(out[0], Le(out[3]));
			EXPECT_THAT(out[1], Le(out[3]));
			EXPECT_THAT(out[2], Le(out[3]));
		}

		TEST_F(Quick_Sort_Test, second_half_is_greater_than_last_element)
		{
			std::vector<int> in{4,5,	0,1,2,	3 };
			std::vector<int> out = sort(in);
			EXPECT_THAT(out[4], Gt(out[3]));
			EXPECT_THAT(out[5], Gt(out[3]));
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
				if (n < 2)
				{
					return in;
				}

				auto left = std::vector<int>(in.begin(), in.begin() + n / 2);
				auto right = std::vector<int>(in.begin() + n / 2, in.end());

				left = operator()(left);
				right = operator()(right);

				left.push_back(std::numeric_limits<int>::max());
				right.push_back(std::numeric_limits<int>::max());
				return merge(left, right, n);

			}
		private:
			std::vector<int> merge(const std::vector<int>& left, const std::vector<int>& right, int n)
			{
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

		TEST_F(Merge_Sort_Test, interleved_two_unsorted_half)
		{
			expect_with_input({ 3,1,4,2 }, { 1,2,3,4 });
		}


	}

}




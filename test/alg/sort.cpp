

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
			std::vector<int> operator ()(const std::vector<int>& in) 
			{
				if (in.size() < 2)
				{
					return in;
				}

				int n = in.size();
				auto left = std::vector<int>(in.begin(), in.begin() + n / 2);
				auto right = std::vector<int>(in.begin() + n / 2, in.end());

				auto sorted_left = operator()(left);
				auto sorted_right = operator()(right);

				return merge(sorted_left, sorted_right);
			}

			std::vector<int> merge(const std::vector<int>& a, const std::vector<int>& b)
			{
				std::vector<int> ret(a.size() + b.size());
				int i = 0, j = 0, k = 0;
				while (k < ret.size())
				{
					if (i < a.size() && j < b.size())
					{
						if (a[i] <= b[j])
						{
							ret[k] = a[i];
							k++;
							i++;
						}
						else
						{
							ret[k] = b[j];
							k++;
							j++;
						}
					}
					else
					{
						if (i < a.size())
						{
							ret[k] = a[i];
							k++;
							i++;
						}

						if (j < b.size())
						{
							ret[k] = b[j];
							k++;
							j++;
						}

					}
				}
				return ret;
			}
		};

		class Merge_Sort_Test :public Test
		{
		protected:
			Merge_Sort sort;

			void expect_result_with_input(const std::vector<int>& in, const std::vector<int>& exp)
			{
				Merge_Sort sort;
				auto act = sort(in);
				EXPECT_THAT(act, Eq(exp));
			}

			void expect_merge_with_input(const std::vector<int>& a, const std::vector<int>& b, const std::vector<int>& exp)
			{
				auto act = sort.merge(a, b);
				EXPECT_THAT(act, Eq(exp));
			}
		};

		TEST_F(Merge_Sort_Test, sort_one_element)
		{
			expect_result_with_input({ 1 }, { 1 });
		}


		TEST_F(Merge_Sort_Test, merge_two_singleton_vectors)
		{
			expect_merge_with_input({ 1 }, { 2 }, { 1,2 });
		}

		TEST_F(Merge_Sort_Test, merge_two_vectors_with_interleave_elements)
		{
			expect_merge_with_input({ 1,3 }, { 2,4 }, { 1,2, 3,4 });
		}

		TEST_F(Merge_Sort_Test, sort_two_element_in_reversed_order)
		{
			expect_result_with_input({ 2,1 }, { 1,2 });
		}

		TEST_F(Merge_Sort_Test, sort_more_1)
		{
			expect_result_with_input({ 2,1 ,4,5}, { 1,2,4,5 });
		}

		TEST_F(Merge_Sort_Test, sort_more_2)
		{
			expect_result_with_input({ 3,2,1 ,4,5}, { 1,2,3,4,5 });
		}

	}

}




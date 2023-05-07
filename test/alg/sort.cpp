

#include "gmock/gmock.h"

#include <vector>
#include <limits>

using namespace testing;

namespace sort
{

	class Insertion_Sort
	{
	public:
		void apply(std::vector<int>& in)
		{
			for (int i = 1; i < in.size(); i++)
			{

				int curr = in[i];
				int j = i - 1;
				while (j >= 0 && curr < in[j])
				{
					in[j + 1] = in[j];
					j--;
				}
				in[j + 1] = curr;
			}
		}

	};

	class Merge_Sort
	{
	public:
		void apply(std::vector<int>& in)
		{
			sort_sub(in, 0, in.size());
		}
	private:
		void sort_sub(std::vector<int>& in, int p, int r)
		{
			if (r - p > 1)
			{
				int q = (p + r) / 2;
				sort_sub(in, p, q);
				sort_sub(in, q, r);
				merge(in, p, q, r);
			}
		}
		void merge(std::vector<int>& in, int p, int q, int r)
		{
			std::vector<int> lv(q - p + 1);
			std::vector<int> rv(r - q + 1);
			for (int i = 0; i < q - p; i++)
			{
				lv[i] = in[p + i];
			}
			lv.back() = std::numeric_limits<int>::max();
			for (int i = 0; i < r - q; i++)
			{
				rv[i] = in[q + i];
			}
			rv.back() = std::numeric_limits<int>::max();


			int li = 0;
			int ri = 0;
			for (int i = p; i < r; i++)
			{
				if (lv[li] < rv[ri])
				{

					in[i] = lv[li];
					li++;
				}
				else
				{
					in[i] = rv[ri];
					ri++;
				}

			}

		}
	};


	TEST(Sort_Test, insertion_sort_321)
	{
		std::vector<int> act{ 3,2,1 };
		Insertion_Sort s;
		s.apply(act);

		std::vector<int> exp{ 1,2,3 };
		EXPECT_THAT(act, Eq(exp));

	}

	TEST(Sort_Test, merge_sort_312)
	{
		std::vector<int> act{ 3,1,2 };
		Merge_Sort s;
		s.apply(act);

		std::vector<int> exp{ 1,2,3 };
		EXPECT_THAT(act, Eq(exp));

	}

	TEST(Sort_Test, merge_sort_3412)
	{
		std::vector<int> act{ 3,4,1,2 };
		Merge_Sort s;
		s.apply(act);

		std::vector<int> exp{ 1,2,3 ,4 };
		EXPECT_THAT(act, Eq(exp));

	}

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
}




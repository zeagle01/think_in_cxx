

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
				return in;
			}
		};

		class Merge_Sort_Test:public Test
		{
		protected:
			Merge_Sort sort;
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
			std::vector<int> in{ 2 };
			auto act = sort(in);
			EXPECT_THAT(act, Eq(in));
		}


	}

}






#include "gmock/gmock.h"

#include <vector>

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
			int q = in.size() / 2;
			sort_sub(in, 0, q);
			sort_sub(in, 1, in.size());
			merge(in, 0, q, in.size());
		}
	private:
		void sort_sub(std::vector<int>& in, int p, int q) 
		{

		}
		void merge(std::vector<int>& in, int p, int q, int r)
		{

		}
	};


}



using namespace testing;
using namespace sort;

TEST(Sort_Test, insertion_sort_321)
{
	std::vector<int> act{ 3,2,1 };
	Insertion_Sort s;
	s.apply(act);

	std::vector<int> exp{ 1,2,3 };
	EXPECT_THAT(act, Eq(exp));

}

TEST(Sort_Test, insertion_sort_312)
{
	std::vector<int> act{ 3,1,2 };
	Insertion_Sort s;
	s.apply(act);

	std::vector<int> exp{ 1,2,3 };
	EXPECT_THAT(act, Eq(exp));

}

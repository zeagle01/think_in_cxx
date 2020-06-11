

#include "gmock/gmock.h"

#include <vector>

namespace sort
{

	class Dummy
	{
		std::vector<int> apply(const std::vector<int>& in)
		{
			return std::vector<int>();
		}

	};


}



using namespace testing;
using namespace sort;

TEST(Sort_Test, two)
{

}
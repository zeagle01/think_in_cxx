

#include "gmock/gmock.h"


namespace bvh
{

	struct vec
	{
		float x;
		float y;
		float z;
	};

	class Bvh_Tree
	{

	public:
		void build(const std::vector<vec>& positions)
		{
			for (int i = 0; positions.size(); i++)
			{

			}
		}

	};

}



using namespace testing;
using namespace bvh;


TEST(bvh, test_build)
{

	std::vector<vec> positions;
	Bvh_Tree bvh;
	bvh.build(positions);

}


#include "gmock/gmock.h"
#include <random>
#include <limits>
#include <numeric>
#include <queue>
#include <tuple>

using namespace testing;

namespace bvh
{

	struct vec
	{
		float x;
		float y;
		float z;
	};


	vec operator+(vec a,vec b)
	{
		return vec(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	struct aabb_box
	{
		aabb_box()
		{
			lower.x = std::numeric_limits<float>::max();
			lower.y = std::numeric_limits<float>::max();
			lower.z = std::numeric_limits<float>::max();

			upper.x = std::numeric_limits<float>::lowest();
			upper.y = std::numeric_limits<float>::lowest();
			upper.z = std::numeric_limits<float>::lowest();
		}

		aabb_box& operator+=(const aabb_box& other)
		{
			lower.x = std::min(other.lower.x, lower.x);
			lower.y = std::min(other.lower.y, lower.y);
			lower.z = std::min(other.lower.z, lower.z);

			upper.x = std::max(other.upper.x, upper.x);
			upper.y = std::max(other.upper.y, upper.y);
			upper.z = std::max(other.upper.z, upper.z);

			return *this;
		}

		aabb_box& operator+=(const vec& v)
		{
			lower.x = std::min(lower.x, v.x);
			lower.y = std::min(lower.y, v.y);
			lower.z = std::min(lower.z, v.z);

			upper.x = std::max(upper.x, v.x);
			upper.y = std::max(upper.y, v.y);
			upper.z = std::max(upper.z, v.z);

			return *this;
		}

		vec lower;
		vec upper;
	};



	std::vector<vec> generate_primitive_position(int primitive_num)
	{
		std::vector<vec> positions(primitive_num);
		std::default_random_engine re;
		for (int i = 0; i < primitive_num; i++)
		{
			positions[i].x = re();
			positions[i].y = re();
			positions[i].z = re();
		}
		return positions;
	}

	void normalize_positions(std::vector<vec>& positions)
	{
		aabb_box global_box;
		for (int i = 0; i < positions.size(); i++)
		{
			global_box += positions[i];
		}

		global_box.upper.x *= 1.05; //widen
		global_box.upper.y *= 1.05;
		global_box.upper.z *= 1.05;

		float dx = global_box.upper.x - global_box.lower.x;
		float dy = global_box.upper.y - global_box.lower.y;
		float dz = global_box.upper.z - global_box.lower.z;

		for (int i = 0; i < positions.size(); i++)
		{
			positions[i].x = positions[i].x / dx;
			positions[i].y = positions[i].x / dy;
			positions[i].z = positions[i].x / dz;
		}

	}

	std::vector<aabb_box> generate_aabb_box(const std::vector<vec>& positions, float width)
	{
		std::vector<aabb_box> ret(positions.size());
		for (int i = 0; i < positions.size(); i++)
		{
			ret[i] += positions[i];
			ret[i] += positions[i] + vec{ 0.5f * width,0.5f * width,0.5f * width };
			ret[i] += positions[i] + vec{ -0.5f * width,-0.5f * width,-0.5f * width };
		}
		return ret;
	}

	std::vector<int> reorder(const std::vector<vec>& positions)
	{
		std::vector<int> ret(positions.size());
		std::iota(ret.begin(), ret.end(), 0);

		//std::shuffle(ret.begin(), ret.end(), std::default_random_engine{});
		return ret;
	}


	struct node
	{
		node* left = nullptr;
		node* right = nullptr;
		int box = -1;
		int range_left = -1;
		int range_right = -1;
	};

	struct bvh
	{
		node* root = nullptr;
	};

	int split(int b, int e)
	{
		return (e + b) / 2;
	}

	node* build_bvh_inner(int b, int e, const std::vector<aabb_box>& aabb_boxes, const std::vector<int>& order)
	{
		if (e - b == 1)
		{
			return new node{ .box = order[b],.range_left = b,.range_right = e };
		}

		int s = split(b, e);
		node* left = build_bvh_inner(b, s, aabb_boxes, order);
		node* right = build_bvh_inner(s, e, aabb_boxes, order);
		return new node{ .left = left,.right = right,.range_left = b,.range_right = e };
	}

	
	bvh build_bvh(const std::vector<aabb_box>& aabb_boxes, const std::vector<int>& order)
	{
		bvh ret;
		ret.root = build_bvh_inner(0, aabb_boxes.size(), aabb_boxes, order);
		return ret;
	}


	std::vector<std::pair<int,int>> range_in_broad_first_travers(const bvh& tree)
	{
		std::vector<std::pair<int,int>> ret;

		std::queue<node*> q;
		q.push(tree.root);
		while (!q.empty())
		{
			auto n = q.front();
			ret.push_back({ n->range_left, n->range_right });
			if (n->left)
			{
				q.push(n->left);
			}
			if (n->right)
			{
				q.push(n->right);
			}
			q.pop();
		}
		return ret;
	}


	using namespace testing;
	using namespace bvh;

	TEST(bvh, test_global_aabb)
	{
		int primitive_num = 100;

		std::vector<vec> positions = generate_primitive_position(primitive_num);

		aabb_box global_box;
		for (int i = 0; i < primitive_num; i++)
		{
			global_box += positions[i];
		}

		for (int i = 0; i < primitive_num; i++)
		{
			EXPECT_THAT(positions[i].x, Le(global_box.upper.x));
			EXPECT_THAT(positions[i].x, Ge(global_box.lower.x));
			EXPECT_THAT(positions[i].y, Le(global_box.upper.y));
			EXPECT_THAT(positions[i].y, Ge(global_box.lower.y));
			EXPECT_THAT(positions[i].z, Le(global_box.upper.z));
			EXPECT_THAT(positions[i].z, Ge(global_box.lower.z));
		}

	}

	TEST(bvh, test_normalize_positions)
	{
		int primitive_num = 100;
		std::vector<vec> positions = generate_primitive_position(primitive_num);

		normalize_positions(positions);

		for (int i = 0; i < primitive_num; i++)
		{
			EXPECT_THAT(positions[i].x, Le(1.));
			EXPECT_THAT(positions[i].x, Ge(0.));
			EXPECT_THAT(positions[i].y, Le(1.));
			EXPECT_THAT(positions[i].y, Ge(0.));
			EXPECT_THAT(positions[i].z, Le(1.));
			EXPECT_THAT(positions[i].z, Ge(0.));
		}

	}

	TEST(bvh, test_generate_aabb_boxes)
	{
		int primitive_num = 100;
		std::vector<vec> positions = generate_primitive_position(primitive_num);
		normalize_positions(positions);
		float width = 0.05f;
		auto aabb_boxes = generate_aabb_box(positions, width);

		for (int i = 0; i < aabb_boxes.size(); i++)
		{
			EXPECT_THAT(aabb_boxes[i].lower.x, Le(1.f + width));
			EXPECT_THAT(aabb_boxes[i].lower.x, Ge(0.f - width));
			EXPECT_THAT(aabb_boxes[i].upper.x, Le(1.f + width));
			EXPECT_THAT(aabb_boxes[i].upper.x, Ge(0.f - width));
			EXPECT_THAT(std::abs(aabb_boxes[i].upper.x - aabb_boxes[i].lower.x - width), Lt(1e-4f));
		}
	}

	TEST(bvh, test_bvh_range)
	{
		std::vector<vec> positions = 
		{
			{0.2,0,0},
			{0.5,0,0},
			{0.8,0,0}
		};

		int primitive_num = positions.size();

		float width = 0.4f;
		auto aabb_boxes = generate_aabb_box(positions, width);
		auto order = reorder(positions);

		auto bvh = build_bvh(aabb_boxes, order);
		auto act = range_in_broad_first_travers(bvh);
		std::vector<std::pair<int, int>> exp{ {0,3},{0,1},{1,3},{1,2},{2,3} };

		EXPECT_THAT(act, exp);
	}
}

#include "gmock/gmock.h"
#include <list>
#include <map>
#include <initializer_list>



using namespace testing;

namespace std_list
{


	TEST(Std_List_Test, add)
	{
		std::list<int> l;
		l.push_back(0);
		l.push_back(1);
		std::vector<int> exp{ 0,1 };

		int i = 0;
		for (auto& it : l)
		{
			EXPECT_THAT(exp[i], it);
			i++;
		}
	}

	struct Order_Map
	{
		std::map<std::string, std::list<int>::iterator> iterators;
		std::list<int> l;

		void add(std::string name , int v)
		{

			l.push_back(v);
			auto back_it = l.end();
			iterators[name] = --back_it;

		}


		void erase(std::string name)
		{
			auto it = iterators[name];
			l.erase(it);
			iterators.erase(name);
		}

		void test_with_exp_vector(std::initializer_list<int> exp)
		{
			std::vector<int> v_exp(exp);
			int i = 0;
			for (auto& it : l)
			{
				EXPECT_THAT(v_exp[i], it);
				i++;
			}

			EXPECT_THAT(l.size(), Eq(iterators.size()));
		}
	};


	TEST(Std_List_Test, Order_Map_test )
	{
		Order_Map order_map;
		order_map.add("a", 0);
		order_map.add("b", 1);
		order_map.add("c", 2);

		order_map.test_with_exp_vector({ 0,1,2 });

		order_map.erase("c");
		order_map.test_with_exp_vector({ 0,1 });

		order_map.add("c", 2);
		order_map.test_with_exp_vector({ 0,1,2 });

		order_map.erase("b");
		order_map.test_with_exp_vector({ 0,2 });

		order_map.add("b", 1);
		order_map.test_with_exp_vector({ 0,2 ,1 });

		order_map.add("d", 3);
		order_map.test_with_exp_vector({ 0,2 ,1 ,3});

	}

}
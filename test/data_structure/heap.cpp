
#include "gmock/gmock.h"

#include <string>
#include <vector>
#include <sstream>
#include <limits>
#include <algorithm>

using namespace testing;

namespace heap_test
{


	class Heap
	{
	private:

		struct Node
		{
			int data;

			Node* parent = nullptr;
			Node* left_child = nullptr;
			Node* right_child = nullptr;
			Node* previous = nullptr;
			Node* next = nullptr;
		};

		Node* top = nullptr;
		Node* tail = nullptr;
	private:

		void append_to_tail(int v)
		{
			Node* new_node = new Node{ v };
			if (tail)
			{
				new_node->previous = tail;
				tail->next = new_node;
				if (tail->parent)
				{
					if (tail == tail->parent->right_child) //tail is right child
					{
						auto new_node_parent = tail->parent->next;
						new_node->parent = new_node_parent;
						new_node_parent->left_child = new_node;
					}
					else if (tail == tail->parent->left_child)//tail is left child
					{
						auto new_node_parent = tail->parent;
						new_node->parent = new_node_parent;
						new_node_parent->right_child = new_node;
					}
				}
				else
				{
					assert(tail->left_child == nullptr);

					auto new_node_parent = tail;
					new_node->parent = new_node_parent;
					new_node_parent->left_child = new_node;
				}

				tail = new_node;
			}
			else
			{
				tail = new_node;
				top = new_node;
			}
		}

		void swap(Node* n0, Node* n1)
		{
			std::swap(n0->data, n1->data);
		}

		void move_tail_up()
		{
			auto current_node = tail;
			while (true)
			{
				if (current_node->parent && current_node->parent->data > current_node->data)
				{
					swap(current_node, current_node->parent);
					current_node = current_node->parent;
				}
				else
				{
					break;
				}
			}
		}

		void put_tail_on_top()
		{
			if (tail != top)
			{
				swap(tail, top);
				auto pre_tail = tail->previous;
				pre_tail->next = nullptr;
				if (tail->parent->left_child == tail)
				{
					tail->parent->left_child = nullptr;
				}
				else if (tail->parent->right_child == tail)
				{
					tail->parent->right_child = nullptr;
				}
				delete tail;
				tail = pre_tail;
			}
			else
			{
				delete top;
				tail = nullptr;
				top = nullptr;
			}
		}

		void move_top_down()
		{
			if (!top)
			{
				return;
			}

			while (true)
			{
				int left_value = top->left_child ? top->left_child->data : std::numeric_limits<int>::max();
				int right_value = top->right_child ? top->right_child->data : std::numeric_limits<int>::max();
				int min_value_of_children = std::min(left_value, right_value);
				if (top->data > min_value_of_children)
				{
					if (left_value <= right_value)
					{
						swap(top, top->left_child);
					}
					else
					{
						swap(top, top->right_child);
					}
				}
				else
				{
					break;
				}
			}
		}

	public: 
		void insert(int v)
		{
			append_to_tail(v);
			move_tail_up();
		}


		void remove_top()
		{
			put_tail_on_top();
			move_top_down();

		}

		int get_top()
		{
			return top->data;
		}

	public:
		std::vector<std::string> get_status()
		{
			std::vector<std::string> ret;
			Node* current_node = top;
			while (current_node)
			{

				std::stringstream ss;
				ss << current_node->data;
				ret.push_back(ss.str());
				current_node = current_node->next;
			}
			return ret;
		}

	};
	

	using status = std::vector<std::string>;

	TEST(Heap_Test, test_insert)
	{
		Heap h;

		h.insert(5);
		EXPECT_THAT(h.get_status(), Eq(status{ "5" }));

		h.insert(2);
		EXPECT_THAT(h.get_status(), Eq(status{ "2","5"}));

		h.insert(1);
		EXPECT_THAT(h.get_status(), Eq(status{ "1","5","2"}));

		h.insert(4);
		EXPECT_THAT(h.get_status(), Eq(status{ "1","4","2","5" }));
	}

	TEST(Heap_Test, test_get_top_is_right)
	{
		Heap h;

		h.insert(5);
		EXPECT_THAT(h.get_top(), Eq(5));

		h.insert(2);
		EXPECT_THAT(h.get_top(), Eq(2));

		h.insert(6);
		EXPECT_THAT(h.get_top(), Eq(2));

		h.insert(1);
		EXPECT_THAT(h.get_top(), Eq(1));
	}

	TEST(Heap_Test, test_delete_top)
	{
		Heap h;
		h.insert(5); h.insert(2); h.insert(1); h.insert(4);//{1425}

		h.remove_top();
		EXPECT_THAT(h.get_status(), Eq(status{ "2","4","5" }));
	}

	TEST(Heap_Test, test_delete_top_with_only_one_element)
	{
		Heap h;
		h.insert(5); 

		h.remove_top();
		EXPECT_THAT(h.get_status(), Eq(status{}));
	}

}

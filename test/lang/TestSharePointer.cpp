

#include "gmock/gmock.h"
#include <memory>
#include <string>
#include <vector>

using namespace testing;

namespace share_ptr_test
{

	class Data
	{
		int32_t* d;
		int32_t my_id;

	public:
		static int32_t id;
		int32_t get_id() {
			return my_id;
		}
		Data()
		{
			d = new int32_t[10];
			my_id = id;
			printf("in Data%d's constructor\n", my_id);
			id++;
		}
		~Data()
		{
			delete[] d;
			printf("in Data%d's destructor\n", my_id);
		}
	};
	int32_t Data::id = 0;


	TEST(TestSharePointer, use_data)
	{
		std::shared_ptr<Data> p;
		{
			std::shared_ptr<Data> p0 = std::make_shared<Data>();
			std::shared_ptr<Data> p1 = p0;
			EXPECT_EQ(p1->get_id(), p0->get_id());
			p0 = std::make_shared<Data>();
			EXPECT_NE(p1->get_id(), p0->get_id());
			std::shared_ptr<Data> p2 = std::make_shared<Data>();
			p = p0;
		}
	}

	namespace share_ptr_void
	{

		static std::vector<std::string> call_stack;
		class A
		{
		public:
			A() { call_stack.push_back("c A"); };
			~A() { call_stack.push_back("d A"); };

		};

		class B
		{
		public:
			B() { call_stack.push_back("c B"); };
			~B() { call_stack.push_back("d B"); };
		};

		TEST(TestSharePointer, share_ptr_void_still_counts)
		{
			std::shared_ptr<void> p;
			{
				std::shared_ptr<A> pa = std::make_shared<A>();
				std::shared_ptr<B> pb = std::make_shared<B>();
				//std::shared_ptr<void> p = std::static_pointer_cast<void>(pa);
				p = std::static_pointer_cast<void>(pa);
			}
			std::vector<std::string> exp{ "c A","c B","d B" };
			EXPECT_THAT(call_stack, Eq(exp));
		}

	}

}

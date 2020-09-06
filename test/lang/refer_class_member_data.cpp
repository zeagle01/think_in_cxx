

#include "gmock/gmock.h"


using namespace testing;


namespace refer_class_member_data
{


	struct With_Int
	{
		int  data;


		int& get_data()
		{
			return data;
		}
	};

	struct With_Vector
	{
		std::vector<int > data;


		const std::vector<int>& get_data() const
		{
			return data;
		}
	};


	template<typename T>
	struct Refer_Member
	{
		Refer_Member(T& data) :data(data) {}
		T& data;
	};




	TEST(Refer_Class_Member_Data, refer_int_shoulde_sync)
	{
		With_Int with_int;
		with_int.data = 10;
		Refer_Member ref{with_int.data};
		with_int.data = 20;

		EXPECT_THAT(ref.data, Eq(20));
	}



	TEST(Refer_Class_Member_Data, refer_vector_shoulde_sync)
	{
		With_Vector with_vector;
		with_vector.data = std::vector<int>();
		Refer_Member ref{with_vector.get_data()};
		with_vector.data.push_back(20);

		EXPECT_THAT(ref.data[0], Eq(20));
	}

	TEST(Refer_Class_Member_Data, refer_vector_after_reconstruct)
	{
		With_Vector with_vector;
		with_vector.data.push_back(20);
		Refer_Member ref{with_vector.get_data()};
		with_vector.data = std::vector<int>();
		with_vector.data.push_back(30);

		EXPECT_THAT(ref.data[0], Eq(30));
	}



}
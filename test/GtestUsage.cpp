

#include "gmock/gmock.h"



using namespace ::testing;





//////////////////////////////////////////////////////////////appetizer
TEST(GtestUsage,test_a_hello_world) {


	EXPECT_THAT(1 + 1, Eq(2));

}


TEST(GtestUsage, test_string_equal) {
	std::string act = std::string("al")+"pha";
	EXPECT_THAT(act, Eq("alpha"));
}


TEST(GtestUsage, test_string_start_with) {
	std::string act = std::string("al")+"pha";
	EXPECT_THAT(act, StartsWith("alp"));
}



//All of
TEST(GtestUsage, test_multiple_condition_with_Allof) {

	std::string act = std::string("al")+"pha";
	EXPECT_THAT(act, AllOf(StartsWith("al"), EndsWith("ha"), Ne("alqha")));
}
//////////////////////////////////////////////////////////////appetizer


//////////////////////////////////////////////////////////////float
TEST(GtestUsage, test_float_equals_legacy) {
	EXPECT_NEAR(0.1f, 0.2f,1e-6);
}

TEST(GtestUsage, test_float_equals_hamcrest) {
	EXPECT_THAT(0.1f, FloatEq(0.2));
}

TEST(GtestUsage, test_float_near_hamcrest) {
	EXPECT_THAT(0.1f, FloatNear(0.2,1e-2));
}

TEST(GtestUsage, test_float_eq_including_nan) {
	//EXPECT_THAT(10.f/0, NanSensitiveFloatEq(10/0.f));
	//EXPECT_THAT(11.f/0, NanSensitiveFloatEq(10/0.f));
}
//////////////////////////////////////////////////////////////float




//////////////////////////////////////////////////////////////container
TEST(GtestUsage, test_vector_equal) {
	std::vector<int> a{1,2};
	std::vector<int> b{1,3};
	EXPECT_EQ(a, b);
}

TEST(GtestUsage, test_vector_equal_with_ElementsAre) {
	std::vector<int> a{1,2};
	EXPECT_THAT(a, ElementsAre(1,3));
}

TEST(GtestUsage, test_vector_equal_with_UnorderedElementsAre) {
	std::vector<int> a{1,2};
	EXPECT_THAT(a, UnorderedElementsAre(2,3));
}
//////////////////////////////////////////////////////////////container

//////////////////////////////////////////////////////////////pointer
TEST(GtestUsage, test_int_pointer) {
	int* a = new int(1);
	EXPECT_THAT(a, Pointee(2));
	delete a;
}

//////////////////////////////////////////////////////////////pointer

//////////////////////////////////////////////////////////////object
namespace UGtest{
	class Foo {
		friend bool operator==(const Foo& l, const Foo& r);

	private:
		int x, y;
	public:
		Foo(int x) :x(x) {}
	};


	bool operator==(const Foo& l, const Foo& r) {
		return l.x == r.x;
	}

}



TEST(GtestUsage, test_Foo_equal) {

	UGtest::Foo a(1);
	UGtest::Foo b(2);

	EXPECT_THAT(a, Eq(b));
}
//////////////////////////////////////////////////////////////object

//////////////////////////////////////////////////////////////custom matcher
MATCHER(IsEven, "") { return (arg % 2) == 0; }
MATCHER_P(IsDivisibleBy,n, "") { return (arg % n) == 0; }
MATCHER_P2(InRange,low,high, "") { return arg<=high&&arg>=low; }
MATCHER_P(NearWithPrecision, ferr, "")
{
	return abs(get<0>(arg) - get<1>(arg)) < ferr;
}


TEST(GtestUsage, test_IsEven) {
	EXPECT_THAT(3, IsEven());
}

TEST(GtestUsage, test_DivisibleBy) {
	EXPECT_THAT(3, IsDivisibleBy(4));
}

TEST(GtestUsage, test_IsInRange) {
	EXPECT_THAT(3, InRange(4,5));
}

TEST(GtestUsage, float_array_near)
{
	std::vector<float> float_array = { 1.100001f, 2.000002f, 3.000003f };
	std::vector<float> ground_truth = { 1.0f, 2.0f, 3.0f };
	EXPECT_THAT(float_array, Pointwise(NearWithPrecision(1e-4), ground_truth));
}


//////////////////////////////////////////////////////////////custom matcher




//////////////////////////////////////////////////////////////fixture
namespace UGtest{

	class Avector :public Test{
	public:
		std::vector<int> v;
		std::vector<int> v_filled;
	protected:
		void SetUp() {
			v_filled.push_back(0);
		}
	};

}

using  UGtest_Avector = UGtest::Avector;
TEST_F(UGtest_Avector, test_is_empty) {
	EXPECT_TRUE(v.empty());
}

TEST_F(UGtest_Avector, test_is_not_empty_after_push) {
	v.push_back(0);
	EXPECT_FALSE(v.empty());
}


TEST_F(UGtest_Avector, test_after_pop_is_empty) {
	v_filled.pop_back();
	EXPECT_TRUE(v.empty());
}

TEST_F(UGtest_Avector, test_is_not_empty) {
	EXPECT_FALSE(v_filled.empty());
}
//////////////////////////////////////////////////////////////fixture





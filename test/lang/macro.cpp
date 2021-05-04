
#include "gmock/gmock.h"

using namespace testing;

namespace macro
{

#define EVAL(...) EVAL256(__VA_ARGS__)
#define EVAL256(...) EVAL128(EVAL128(__VA_ARGS__))
#define EVAL128(...) EVAL64(EVAL64(__VA_ARGS__))
#define EVAL64(...) EVAL32(EVAL32(__VA_ARGS__))
#define EVAL32(...) EVAL16(EVAL16(__VA_ARGS__))
#define EVAL16(...) EVAL8(EVAL8(__VA_ARGS__))
#define EVAL8(...) EVAL4(EVAL4(__VA_ARGS__))
#define EVAL4(...) EVAL2(EVAL2(__VA_ARGS__))
#define EVAL2(...) EVAL1(EVAL1(__VA_ARGS__))
#define EVAL1(...) __VA_ARGS__

#define LOOP_0(PRE_MACRO,MACRO,...) 
#define LOOP_1(PRE_MACRO,MACRO,...) LOOP_0(PRE_MACRO,MACRO,__VA_ARGS__) PRE_MACRO(0,__VA_ARGS__)
#define LOOP_2(PRE_MACRO,MACRO,...) LOOP_1(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(1,__VA_ARGS__)
#define LOOP_3(PRE_MACRO,MACRO,...) LOOP_2(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(2,__VA_ARGS__)
#define LOOP_4(PRE_MACRO,MACRO,...) LOOP_3(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(3,__VA_ARGS__)


#define LOOP(LOOP_COUNT,PRE_MACRO,MACRO,...) LOOP_##LOOP_COUNT(PRE_MACRO,MACRO,__VA_ARGS__)

#define EMP()
#define DEFF(m) m EMP()
#define _LOOP() LOOP
#define INSIDE_LOOP(LOOP_COUNT,PRE_MACRO,MACRO,...) DEFF(_LOOP)()(LOOP_COUNT,PRE_MACRO,MACRO,__VA_ARGS__)
















/////////////////test/////////////////




#define PUSH_BACK_LIST(i,v) v.push_back(i);

	TEST(Macro_Test, single_loop_with_index)
	{
		std::vector<int > v;
		LOOP(3, PUSH_BACK_LIST, PUSH_BACK_LIST, v);


		std::vector<int > exp{ 0,1,2 };
		EXPECT_THAT(v, Eq(exp));

	}



#define PUSH_BACK_LIST_OF_LIST(i,v) INSIDE_LOOP(i,PUSH_BACK_LIST,PUSH_BACK_LIST,v)

	TEST(Macro_Test, double_loop_with_index)
	{
		std::vector<int> v;
		EVAL(LOOP(4, PUSH_BACK_LIST_OF_LIST, PUSH_BACK_LIST_OF_LIST, v));


		std::vector<int> exp
		{
			0,
			0,1 ,
			0,1 ,2
		};

		EXPECT_THAT(v, Eq(exp));

	}

}

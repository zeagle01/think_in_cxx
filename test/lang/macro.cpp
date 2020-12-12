
#include "gmock/gmock.h"


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


#define SECOND(a,b,...) b
#define IS_PROBE(...) EVAL1(SECOND(__VA_ARGS__,0))
#define PROBE() ~,1

#define _NOT_0 PROBE()
#define NOT(x) IS_PROBE(_NOT_ ## x)

#define CAT(x,y) x##y

#define IF_ELSE_VV(condition) CAT(_IF_,condition )  
#define IF_ELSE(condition) IF_ELSE_VV(NOT(condition))

#define _IF_1(...) EVAL1(__VA_ARGS__ _IF_1_ELSE)
#define _IF_0(...)             _IF_0_ELSE

#define _IF_1_ELSE(...)
#define _IF_0_ELSE(...) __VA_ARGS__

#define EMPTY()


#define DEFER1(m) m EMPTY()
#define DEFER2(m) m EMPTY EMPTY()()
#define DEFER3(m) m EMPTY EMPTY EMPTY()()()
#define DEFER4(m) m EMPTY EMPTY EMPTY EMPTY()()()()



#define FIRST(a, ...) a

#define HAS_ARGS(...) NOT(FIRST(_END_OF_ARGUMENTS_ __VA_ARGS__)())
//#define HAS_ARGS_VV(...) EXP(FIRST(_END_OF_ARGUMENTS_ ,__VA_ARGS__)())
#define _END_OF_ARGUMENTS_() 0

#define MAP(m, first, ...)           \
  m(first)                           \
  IF_ELSE(NOT(first))(    \
    DEFER3(_MAP)()(m, __VA_ARGS__)   \
  )(                                 \
    /* Do nothing, just terminate */ \
  ) \

#define _MAP() MAP



#define AAA(n)  printf("%d\n",n);






	TEST(Macro_Test, loop)
	{
		EVAL(MAP(AAA, 4, 3, 2, 1, 0))
	}


}

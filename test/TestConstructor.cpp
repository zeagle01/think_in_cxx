
#include "gtest/gtest.h"

class A{
    public:
	int i;
	A(){
	    i=0;
	    printf("in constructor\n");
	}
	A(const A& other){

	    printf("in copy constructor\n");
	}
	A(const A&& other){
	    i=std::move(other.i);
	    printf("in move constructor\n");
	}


	void operator=(const A& other){
	    printf("in operator= \n");
	}

	void operator=(const A&& other){
	    i=std::move(other.i);
	    printf("in move operator= \n");
	}

};

TEST(TestConstructor,test_operator_equal){
    A a1;
    A a2;
    a2=a1;
}


TEST(TestConstructor,test_copy_constructor){
    A a1;
    A a2=a1;
}

  A f(A u)
  {
      A v(u);
      A w = u;
      return w;
  }



TEST(TestConstructor,test_copy_function){
    A x;
    A y=f(f(x));

}


A f1(){
    A a;
    return a;
}

TEST(TestConstructor,test_copy_elision){
    A x=f1();
}




TEST(TestConstructor,test_move){
    A x;
    A y;
    x=std::move(y);
    A z(std::move(x));

}



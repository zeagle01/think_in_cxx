

#include "gtest/gtest.h"





class B{
    public:
    void f(){}

};


class A:public B{

    private:
    void f(){}

};






TEST(_19_03_32_Test,test_use){
    A a;
}


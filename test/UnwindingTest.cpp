#include "gtest/gtest.h"
#include <iostream>

class A1
{

  public:
    A1(){
        std::cout << "A1's constructor" << std::endl;
    }
    ~A1()
    {
        std::cout << "A1's destructor" << std::endl;
    }
};

class A2
{
  public:

    A2(){
        std::cout << "A2's constructor" << std::endl;
    }
    ~A2()
    {
        std::cout << "A2's destructor" << std::endl;
    }
};



 void ff1() throw(int)
{
    A1 a1;
    std::cout << "f1() start" << std::endl;
    throw 100;
    //后面的逻辑不会执行，但是a1的析构会执行
    std::cout << "f1() end" << std::endl;
}

void ff2()
{
    A2 a2;
    std::cout << "f2() start" << std::endl;
    try
    {
        ff1();
    }
    catch (int e)
    {
        std::cout << "caught exception " << e << std::endl;
    }
    std::cout << "f2() end" << std::endl;
}

TEST(UnwindingTest, program_flow_and_local_class_destructor)
{
    ff2();
}


class C1{
    public:
    C1()
    {
        std::cout<<"C1() begin"<<std::endl;
        throw 1;
        std::cout<<"C1() end"<<std::endl;
    }
    ~C1(){

        std::cout<<"~C1() end"<<std::endl;
    }

};


class D1{
    public:
    D1()
    {
        std::cout<<"D1() begin"<<std::endl;
        a1=A1();
        c=C1();
        a2=A2();
        std::cout<<"D1() end"<<std::endl;
    }
    ~D1(){

        std::cout<<"~D1() end"<<std::endl;
    }
    private:
    A1 a1;
    A2 a2;
    C1 c;

};

TEST(UnwindingTest,test_constructor_throw){
    try{
    D1 d;
    }
    catch(int i){
        std::cout<<"caught"<<std::endl;
    }

}

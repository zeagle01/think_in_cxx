
#include "gtest/gtest.h"
using namespace std;

class A {
    public:
        A() {
            
        }
        ~A() {
            
        }
};

class B {
    public:
        A mA;
        A *mA2;
        static A mA3;
        static A *mA4;
        static const A mA5;
        static constexpr A * mA6 = nullptr;
        
        B() : mA(), mA2(new A()) {
        }
        
        void print() {
            A mA3;
            A *mA4 = new A();
            A mA5;
            A * mA6  = new A();
            
            std::cout << "B::mA3  " << &mA3 << std::endl;  // 类初始化在哪就在哪
            std::cout << "B::mA4  " << mA4 << std::endl; // 堆上
            std::cout << "B::mA5  " << &mA5 << std::endl;  // 类初始化在哪就在哪
            std::cout << "B::mA6  " << mA6 << std::endl; // 堆上
        }
};

A a3; // 全局变量
A *a4 = new A(); 
static A a5; // 全局静态变量
static A *a6 = new A(); 

B b3;
B *b4 = new B();

TEST(VariableTest,test_use){

    A a1;   // 局部变量
    A *a2 = new A(); 
    static A a7; // 静态局部变量
    static A *a8 = new A();
    
    std::cout << "a1  " << &a1 << std::endl; // 0x7ffeeca846d8 栈地址
    std::cout << "a2  " << a2 << std::endl; // 0x7fb9054022f0 堆地址
    std::cout << "&a2  " << &a2 << std::endl; // 0x7ffeeca846d0 栈地址
    
    std::cout << "a3  " << &a3 << std::endl; // 0x103019110 静态存储区， .data?
    std::cout << "a4  " << a4 << std::endl; // 0x7f921dc02af0 堆上
    std::cout << "&a4  " << &a4 << std::endl; // 0x10b2fd130 静态存储区
    std::cout << "a5  " << &a5 << std::endl; // 0x10b2fd150 静态存储区
    std::cout << "a6  " << a6 << std::endl; // 0x7fc44f502690 堆上
    std::cout << "&a6  " << &a6 << std::endl; // 0x10b2fd158 静态存储区
    std::cout << "a7  " << &a7 << std::endl; // 0x10b2fd160 静态存储区
    std::cout << "a8  " << a8 << std::endl; // 0x7fc44f5026e0 堆上
    std::cout << "&a8  " << &a8 << std::endl; // 0x10b2fd170 静态存储区
    
    B b1;
    std::cout << "b1  " << &b1 << std::endl; // 0x7ffeeca846b0 栈地址
    std::cout << "&b1.mA  " << &b1.mA << std::endl; // 0x7ffeeca846b0 栈地址
    std::cout << "b1.mA2  " << b1.mA2 << std::endl; // 0x7fb905402300 堆地址
    b1.print();
    
    
    B *b2 = new B();
    std::cout << "b2  " << b2 << std::endl; // 0x7fb905402310 堆地址
    std::cout << "&b2.mA  "<< &b2->mA << std::endl; // 0x7fb905402310 堆地址
    std::cout << "b2.mA2  "<< b2->mA2 << std::endl; // 0x7fb905402320  堆地址
    b2->print();
    delete a2;
}






#include "gtest/gtest.h"
#include <iostream>
#include <functional>

using f_t=void ();
using fp_t=void (*)();
using std_f_t=std::function<void()>;

void func(){
    std::cout << " in f" << std::endl;
}
class A{
    public:
    void member_func(){
        std::cout << " in A::f" << std::endl;
    }

};

//using 
void foward_with_pointer(fp_t f){
    std::cout<<"forwarding with function pointer"<<std::endl;
    f();
}

void foward_with_std_function(std_f_t f){
    std::cout<<"forwarding with std::function"<<std::endl;
    f();
}


std::function<void()> create_std_f(){
    std::cout<<"creating std function"<<std::endl;
    return func;
}

fp_t create_fp(){
    std::cout<<"creating fp"<<std::endl;
    return func;
}

TEST(function_pointer_test,test_fowarding){

    foward_with_pointer(func);
    foward_with_std_function(func);

//  these two won't pass
    //foward_with_pointer(A::member_func);
    //foward_with_std_function(A::member_func);
}

TEST(function_pointer_test,test_create){
    std_f_t std_f=create_std_f();
    std_f();

    fp_t fp=create_fp();
    fp();
}

TEST(function_pointer_test,test_pass_fp_2_std_function){
    fp_t fp=func;
    foward_with_std_function(fp);

    ////not the other way around!
    //std_f_t std_f=func;
    //foward_with_pointer(std_f);
}

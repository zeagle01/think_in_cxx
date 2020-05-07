

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




/////////////////////member method////////////////////////////
class A_With_Method_Pointer {

public:
	A_With_Method_Pointer(): 
		f_t(&A_With_Method_Pointer::method)
	{

		std_f_t = std::bind(&A_With_Method_Pointer::method, this);
		std_f_t_1 = [=]() {
			this->method();
		};
	}
	void fowarder() {
		(this->*(this->f_t))();
		std_f_t();
		std_f_t_1();
	}

	void method() {
		std::cout << " in method" << std::endl;
	}
private:
	std::function<void()> std_f_t;
	std::function<void()> std_f_t_1;
	std::function<void()> std_f_t_2;
	void (A_With_Method_Pointer::*f_t)();

};

//////////// use outside of a class//////////////
void outside_fowarder() {

	A_With_Method_Pointer a;

	std::function<void()> std_f_t=std::bind(&A_With_Method_Pointer::method,&a);
	std::function<void()> std_f_t_1=[&]() {
		a.method();
	};

	std_f_t();
	std_f_t_1();


}

TEST(method_pointer, test_foward_method_inside_class) {
	A_With_Method_Pointer a;
	a.fowarder();
}

TEST(method_pointer, test_foward_method_outside_class) {
	outside_fowarder();
}




#include "gtest/gtest.h"

class A{
    public:
	int32_t a=0;
	virtual void f(){
	    a++;
	    printf("%d\n",a);
	}

};


class ADerived:public A{
    public:
	int32_t b=0;
	virtual void f(){
	    b++;
	    printf("%d\n",b);
	}

};


class B{
    public:
	void f(){
	    printf("hi\n");
	}

};

TEST(Pointer,test_delete_class_witouth_data_member){
    B* b=new B();
    b->f();
    delete b;
    b->f();

}

TEST(Pointer,test_delete_class_with_data_member){
    A* a=new A();
    a->f();
    delete a;
    //a->f();

}

void deleteA(A* a){
    delete a;
}

TEST(Pointer,test_delete_twice){
    A* a=new A();
    deleteA(a);
//    deleteA(a);



}


TEST(Pointer,test_bad_access){

    A* a;
    a=new A();
    a->f();
    delete a;
	
//    int* i=new int;
//    a=(A*)i;
//    a->f();



}


TEST(Pointer,test_copyable_case){

    A* p=new A();
    p->f();
    delete p;
    p=nullptr;

    A a=A();
    a.f();


}











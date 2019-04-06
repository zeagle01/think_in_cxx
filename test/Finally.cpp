
#include "gtest/gtest.h"

#include <functional>
#include <iostream>

class Finally{
    public:
    Finally(std::function<void(void)> releaseFunc):f(releaseFunc){
    }
    ~Finally(){
        f();
        std::cout<<"in finally"<<std::endl;

    }
    private:

    std::function<void(void)> f;

};
#include <exception>
class MyException:public std::exception{
    public:
//MyException(){};
//virtual ~MyException(){};
    const char* what() const noexcept override  {
        return "in myexception";
    }

};

TEST(FinallyTest,test_use){

    int32_t* a=new int32_t[2];

    Finally([&](){
        delete[] a;
    });

    try{
        std::cout<<a[0]<<std::endl;

    }catch(std::exception& e){
        std::cout<<e.what()<<std::endl;

    }



}

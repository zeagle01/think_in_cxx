


#include "gtest/gtest.h"
#include <iostream>




class Overload{
    public:
      void f() & {
          std::cout<<"f()"<<std::endl;
      };
      void f() const &{
          std::cout<<"f()const &"<<std::endl;
      };
      void f() && {
          std::cout<<"f()&&"<<std::endl;
      };
      void i() & {
          std::cout<<"i()&"<<std::endl;
      };
};




TEST(OverloadTest,test_use){
    Overload a;
    a.f();
    
    const Overload b;
    b.f();


   Overload c;
   std::move(c).f();

}





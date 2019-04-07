

#include "gtest/gtest.h"
#include <iostream>

class Return_Obj
{
  public:
    Return_Obj()
    {
        std::cout << get_address() << " in ctor" << std::endl;
    };
    Return_Obj(const Return_Obj &other)
    {

        std::cout << get_address() << " in copy ctor" << std::endl;
    }
    Return_Obj &operator=(const Return_Obj &other)
    {
        std::cout << get_address() << " in operator =" << std::endl;
        return *this;
    }
    ~Return_Obj()
    {
        std::cout << get_address() << " in dtor" << std::endl;
    }

  private:
    void *get_address()
    {
        return static_cast<void *>(this);
    }
};

void parameter_f(Return_Obj obj)
{
    std::cout<<"in parameter_f"<<std::endl;
    return;
}
Return_Obj return_f()
{
    std::cout<<"in return_f()"<<std::endl;
    return Return_Obj();
}

TEST(FunctionReturnValueCauseCopy, test_use_as_parameter)
{

    Return_Obj obj;
    parameter_f(obj);
}

TEST(FunctionReturnValueCauseCopy, test_return_obj)
{
    return_f();
    //why doesn't it have a copy constructor here??
    Return_Obj obj = return_f();

    Return_Obj obj1 { return_f()};
}
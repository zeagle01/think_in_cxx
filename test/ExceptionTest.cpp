

#include "gtest/gtest.h"
#include <exception>
#include <iostream>

void f()
{
    try
    {
        int c = 1 / 0;
        //此处指定要抛出的异常
        throw std::runtime_error("division by zero");
        //抛出后的逻辑将不再执行
    }
    //异常抛出后会在此catch块(或上层调用的catch块)查看异常类型是否匹配，若匹配则程序流程进入该catch块
    catch (std::runtime_error &e)
    {
        std::cout << "handle exception" << std::endl;
        std::cout << e.what() << std::endl;
    }
}

TEST(ExceptionTest, test_try_catch)
{
    f();
}





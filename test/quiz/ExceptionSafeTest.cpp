
#include "gtest/gtest.h"
#include <iostream>

class StateMaintainer
{
    public:
    static const int32_t MAX_SIZE=10;
    StateMaintainer()
    {
        init();
    }
    //make sure no exception!
    void perfectUpdate() noexcept{

        for(int32_t i=0;i<MAX_SIZE;i++){
                data[i] = i;
        }

    }
    void goodUpdate(){
        //save original state
        upatePrevious();

        for(int32_t i=0;i<MAX_SIZE;i++){
            try{
                //some random index may throw exception
                if(i==5){
                    throw i;
                }   
                data[i] = i;
            }
            catch(int32_t i){
                std::cout<<"exception at "<<i<<" item"<<std::endl;
                //set the state to original
                restore();
 
            }
        }
    }
    void badUpdate(){
        for(int32_t i=0;i<MAX_SIZE;i++){
            try{
                //some random index may throw exception
                if(i==5){
                    throw i;
                }   
                data[i] = i;
            }
            catch(int32_t i){
                //just report a exception nothing else to repair the state inconsitency
                std::cout<<"exception at "<<i<<" item"<<std::endl;
            }
        }
    }
    void weakUpdate(){
        for(int32_t i=0;i<MAX_SIZE;i++){
            try{
                //some random index may throw exception
                if(i==5){
                    throw i;
                }   
                data[i] = i;
            }
            catch(int32_t i){
                std::cout<<"exception at "<<i<<" item"<<std::endl;
                //reset the state to its initial,won't hurt,but not good
                init();
            }
        }
    }

    private:
    void init(){
        for(int32_t i=0;i<MAX_SIZE;i++){
            data[i]=0;
        }
    }
    void restore(){
        for(int32_t i=0;i<MAX_SIZE;i++){
            data[i]=previous[i];
        }
    }
    void upatePrevious(){
        for(int32_t i=0;i<MAX_SIZE;i++){
            previous[i]=data[i];
        }

    }
    ///void assign(const int32_t& )
    int32_t data[MAX_SIZE];
    int32_t previous[MAX_SIZE];
};


TEST(ExceptionSafeTset, test_strong_ensurance)
{
    StateMaintainer f;
    f.goodUpdate();
}

TEST(ExceptionSafeTset, test_weak_ensurance)
{
    StateMaintainer f;
    f.weakUpdate();
}

TEST(ExceptionSafeTset, test_no_ensurance)
{
    StateMaintainer f;
    f.badUpdate();
}

TEST(ExceptionSafeTset, test_no_exception)
{
    StateMaintainer f;
    f.perfectUpdate();
}
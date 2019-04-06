


#include "gtest/gtest.h"
#include <iostream>

template<typename T>
class Math{
    public:

//	Math(){
//	    mSize=0;
//	    mResult=0;
//	}
//	Math(const int32_t& mSize):mSize(mSize){
//	    mResult=0;
//	}
	static T addTo(int32_t mSize){
	    T mResult=0;
	    for(int32_t i=1;i<mSize+1;i++){
		mResult+=i;
	    }
	    return mResult;
	}
	static T divide(const T& numerator,const T& denominator){
	    if(denominator==T(0)){
//		throw std::run_time("");
	    }
	    return numerator/denominator;
	}

//    private:
//	T mSize;
//	T mResult;

};


TEST(VolatileTest,test_adder){
    Math<int32_t> math;
    std::cout<<"result is "<<math.addTo(100)<<std::endl;
    std::cout<<"result is "<<math.divide(100,5)<<std::endl;

}

TEST(VolatileTest,test_use){
    volatile int32_t a ;


    int32_t result=0;
    const int32_t size=100;
    for(int32_t i=1;i<size+1;i++){
	result+=i;
    }

    std::cout<<"result is "<<result<<std::endl;

}

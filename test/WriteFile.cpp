
#include "gtest/gtest.h"
#include <thread>
#include <fstream>
#include <sstream>
//#include 



void print(std::ofstream* fout,int32_t i ){
		(*fout)<<i+1<<std::endl;
}


TEST(MultiThread,test_write_file){

    int32_t tn=4;
    int32_t fn=4;

    std::vector<std::thread*> threads(tn);
    std::vector<std::ofstream*> fouts(fn);


    for(int32_t i=0;i<4;i++){
	std::stringstream ss;
	ss<<i+1<<".txt";
	fouts[i]=new std::ofstream(ss.str());
	threads[i]=new std::thread(print,fouts[i],i);
	threads[i]->join();
    }

    for(int32_t it=0;it<100;it++){
//	threads[i]->apply();


    }
    


    EXPECT_TRUE(true);

}








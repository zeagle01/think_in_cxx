#include "gtest/gtest.h"

#include <mutex>
#include <queue>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <iostream>


std::mutex  mut;
std::condition_variable cv;
std::queue<int> data;
std::vector<int> data_src{-1,0,1,2,3,-1,-1,5,6};
int processed_in_data_src=0;

int actual_processed_num=0;
int max_actual_processed_num=5;
std::vector<int> actual_proccessed_data;

bool has_more_data(){
    return processed_in_data_src<data_src.size();
}
int prepare_data_from_src(){
    return data_src[processed_in_data_src++];
}

void prepare_data_thread(){

    while(has_more_data()){
        int d = prepare_data_from_src();
        if(d<0){
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }
        std::lock_guard<std::mutex> lg(mut);
        data.push(d);
        cv.notify_one();
    }

}

void process_data(int i){
    //std::this_thread::sleep_for(std::chrono::seconds(1));
    actual_proccessed_data.push_back(i);
}


void process_data_thread(){

    while(true){
        std::unique_lock<std::mutex> ul(mut);
        cv.wait(ul,[](){
            return data.size()>0;
        });
        int d=data.front();
        data.pop();
        ul.unlock();
        process_data(d);

        if (actual_processed_num >= max_actual_processed_num-1)
        {
            break;
        }
        actual_processed_num++;
    }
}








TEST(Chp4_L0401_Test,conditional_variable){

    std::thread producer(prepare_data_thread);
    std::thread consumer(process_data_thread);
    producer.join();
    consumer.join();

    std::for_each(std::begin(actual_proccessed_data),std::end(actual_proccessed_data),
    [](int i){
        std::cout<<"actual processed dadta: "<<i<<std::endl;
    });

//    std::copy(std::begin(actual_proccessed_data),std::end(actual_proccessed_data),std::cout);

    std::vector<int> exp_process_data{0,1,2,3,5};
    EXPECT_EQ(actual_proccessed_data,exp_process_data);

}





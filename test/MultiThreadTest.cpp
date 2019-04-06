

#include "gtest/gtest.h"
#include <thread>
#include <iostream>
#include <vector>
#include <mutex>
#include  <chrono>

std::mutex mu;

void printNumberWithLock(int32_t tid, int32_t size)
{
    for (int32_t i = 0; i < size; i++)
    {
        mu.lock();
        std::cout << "in " << tid << " " << i << std::endl;
        mu.unlock();
    }
}

void printNumberWithoutLock(int32_t tid, int32_t size)
{
    for (int32_t i = 0; i < size; i++)
    {
        mu.lock();
        std::cout << "in " << tid << " " << i << std::endl;
        mu.unlock();
    }
}

void threadHello(){
    std::cout<<"hello concurrent world"<<std::endl;

}

TEST(MultiThreadTest, test_hello_world)
{
    //    std::thread t([]() {
    //        std::cout << "hello world!" << std::endl;
    //    });
    std::thread  t(threadHello);
    t.join();
}

TEST(MultiThreadTest, test_print_without_lock)
{

    std::vector<std::thread> threads(4);
    for (int32_t i = 0; i < threads.size(); i++)
    {
        threads[i] = std::thread(printNumberWithoutLock, i, 4);
    }
    std::cout << "launch from main" << std::endl;
    for (int32_t i = 0; i < threads.size(); i++)
    {
        threads[i].join();
    }
}

TEST(MultiThreadTest, test_print_with_lock)
{

    std::vector<std::thread> threads(4);
    for (int32_t i = 0; i < threads.size(); i++)
    {
        threads[i] = std::thread(printNumberWithLock, i, 4);
    }
    mu.lock();
    std::cout << "launch from main" << std::endl;
    mu.unlock();
    for (int32_t i = 0; i < threads.size(); i++)
    {
        threads[i].join();
    }
}


void print_thread_id(){
    std::cout<<"in slave thread:"<<std::this_thread::get_id()<<std::endl;
    //std::this_thread::sleep_for(std::chrono::seconds(1));
}
TEST(MultiThreadTest, thread_id){
    std::cout<<"in main thread:"<<std::this_thread::get_id()<<std::endl;
    std::thread t(print_thread_id);
    std::cout<<"in slave thread:"<<t.get_id()<<std::endl; 
    t.join();

}

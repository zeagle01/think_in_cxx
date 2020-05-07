

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









//condition variable

#include <mutex>
#include <condition_variable>
#include <thread>

std::condition_variable cv;
std::mutex mt;
bool work2_done=false;
bool work1_done=false;

void work1() {
	std::unique_lock<std::mutex> lock(mt);
	cv.wait(lock, [&]() {return work2_done; });
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	std::cout << "work1 done!" << std::endl;
}

void work2() {
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	std::cout << "work2 done!" << std::endl;

	std::lock_guard<std::mutex> lock(mt);
	work2_done = true;
	cv.notify_one();
}

TEST(MultiThreadTest, condition_variable_test) {

	std::thread  t1(work1);
	std::thread  t2(work2);
	t1.join();
	t2.join();
	std::cout << "main thread done" << std::endl;
}





















#include "gtest/gtest.h"
//#include "gtest/gock.h"
#include <iostream>
#include <thread>
#include <functional>
#include <chrono>
#include <memory>



class some_task{
    public:
    void operator()(){
        std::cout<<"i'm here"<<std::endl;
    }

    void print(){
        std::cout<<"in print"<<std::endl;
    }

};



TEST(Chp2Test,test_callable_obj){
    //some_task task;
    //std::thread t(task);
    std::thread t{some_task()};
    //std::thread t(some_task());//this is ambiguous to function declaration
    t.join();
}



TEST(Chp2Test,test_lambda){
    std::thread t([](){
        std::cout<<"in sub thread!"<<std::endl;
    });
    t.join();
}

class use_local_variable{
    private:
    int32_t &i;
    public:
      use_local_variable(int32_t & i) : i(i){

      };
      void operator()(){
          std::this_thread::sleep_for(std::chrono::seconds(1));
          std::cout<<"pass from local variable:"<<i<<std::endl;
          //std::cout<<i<<std::endl;
      }
};

TEST(Chp2Test,test_use_local_variable_when_function_exit){
    int32_t i=1;
    std::thread t{use_local_variable(i)};

    t.detach();

}

class thread_guard{
    private:
    std::shared_ptr<std::thread> t;
    public:
      thread_guard(std::shared_ptr<std::thread> t):t(t) {}
      ~thread_guard(){
          if(t->joinable()){
              t->join();
              std::cout<<"thread "<< t->get_id()<<"is jointed"<<std::endl;
          }
      }
      thread_guard(const thread_guard &other) = delete;
      thread_guard& operator =(const thread_guard &other)=delete;
};


TEST(Chp2Test,make_sure_the_thread_is_joined_before_function_exit){
    int32_t i=1;
    thread_guard tg(std::make_shared<std::thread>(use_local_variable(i)));
    throw 1;
}



class scoped_thread{
    private:
    std::thread t;
    public:
    scoped_thread(std::thread t) : t(std::move(t)) {
        if(!this->t.joinable()){
            throw std::logic_error("there is no thread!");
        }
        
    }
    ~scoped_thread(){
        t.join();
    }
    scoped_thread (scoped_thread const &other) = delete;
    scoped_thread& operator=(scoped_thread const &other) = delete;

};


void add_number_some_times(int* number,int times){
    for(int i=0;i<times;++i){
        (*number)++;
    }
}

TEST(Chp2Test,spawn_some_threads){
    std::vector<int> data_in{1,2,3,4};
    std::vector<int> data{data_in};
    int times =10;
    std::vector<std::thread> threads;
    for(int i=0;i<data.size();++i){
        threads.push_back(std::thread(add_number_some_times, &data[i], times));
    }
    std::for_each(std::begin(threads),std::end(threads),std::mem_fn(&std::thread::join));

    std::vector<int> exp_out(data.size());
    for(int i=0;i<exp_out.size();++i){
        exp_out[i] = data_in[i] + times;
    }
    EXPECT_EQ(data, exp_out);
}






#include "gtest/gtest.h"
#include <iostream>
#include <thread>
#include <numeric>
#include <functional>

template<typename iterator,typename T>
void accumulate_block(iterator b,iterator e,T* ret){
    for(iterator it=b;it!=e;++it){
        ret[0] += *it;
    }
};


template<typename iterator,typename T>
T parallel_accumulate(iterator b,iterator e,T init){
    int n=std::distance(b,e);
    if(n==0){
        return init;
    }
    int hardware_thread_num=std::thread::hardware_concurrency();
    std::cout<<"I have "<<hardware_thread_num<<" cores fore concurrency!"<<std::endl;

    //int block_size=(n+hardware_thread_num-1)/hardware_thread_num;
    int block_size=n/hardware_thread_num;
    std::cout<<"each process "<<block_size<<" out of "<<n<<" iterms"<<std::endl;
    //int block_size=1;

    std::vector<T> partial_sum(hardware_thread_num);

    std::vector<std::thread> sub_threads(hardware_thread_num-1);
    for(int i=0;i<sub_threads.size();++i){
        iterator thread_begin=b+i*block_size;
        iterator thread_end=thread_begin+block_size;
        sub_threads[i] = std::thread(accumulate_block<iterator,T>,thread_begin,thread_end,&partial_sum[i]);
    }
    accumulate_block<iterator, T>(b + (hardware_thread_num - 1) * block_size, e, &partial_sum.back());
    std::for_each(std::begin(sub_threads),std::end(sub_threads),std::mem_fn(&std::thread::join));

    for(int i=0;i<hardware_thread_num;++i){
        std::cout<<"thread "<<i<<" partial sum is "<<partial_sum[i]<<std::endl;
    }

    T sum=std::accumulate(std::begin(partial_sum),std::end(partial_sum),init);
    return sum;
}

TEST(Chp2_L0208_Test,dummy_accumulate){
    int size=64;
    int init_value=0;
    std::vector<int> data_in(size,1);
    
    int sum=parallel_accumulate(std::begin(data_in),std::end(data_in),init_value);
    std::cout<<sum<<std::endl;
    EXPECT_EQ(sum,size+init_value);
}





#include "gtest/gtest.h"
#include <iostream>


template <typename It>
It high(It b,It e){

    It ret=b;
    for(It i=b;i!=e;++i){
        if(*i>*ret){
            ret=i;
        }
    }
    return ret;
} 





TEST(_15_3_1_Test,test_use){

   int32_t a[]{1,2,3};
   int32_t ha=*high(std::begin(a),std::end(a));
   std::cout<<"int arr's max is "<<ha<<std::endl;

   std::vector<int32_t> b{1,2,3};
   int32_t hb=*high(std::begin(b),std::end(b));
   std::cout<<"int vector's max is "<<hb<<std::endl;

   float fa[]{1,2,3};
   float hfa=*high(std::begin(a),std::end(a));
   std::cout<<"float arr's max is "<<hfa<<std::endl;

   std::vector<float> fb{1,2,3};
   float hfb=*high(std::begin(b),std::end(b));
   std::cout<<"float vector's max is "<<hfb<<std::endl;


}



template<typename T>
class Item{
    public:
    T v=0;
    Item<T> *succ=nullptr;
    Item<T> *pre=nullptr;
};

template <typename T>
class MyList
{
  private:
    Item<T> *dummy;
  public:
    MyList()
    {
         dummy=new Item<T>();
         dummy->succ = dummy;
         dummy->pre = dummy;
    }
    class Iterator
    {
      private:
        Item<T> *item;

      public:
        Iterator(Item<T>* item) : item(item) {}
        Iterator &operator++()
        {
            item=item->succ;
            return *this;
        }
        T& operator*(){
            return item->v;
        }
        bool operator!=(const Iterator& other){
            return item!=other.item;
        }
    };

    Iterator begin(){
        return Iterator(dummy->succ);
    };
    Iterator end(){
        return Iterator(dummy);
    };
    void push_front(T v){
        Item<T>* to_be_pushed=new Item<T>();
        to_be_pushed->v = v;
        to_be_pushed->succ = dummy->succ;
        to_be_pushed->pre = dummy;
        dummy->succ->pre=to_be_pushed;
        dummy->succ = to_be_pushed;
    };
    void erase(Iterator it){};
    void insert(Iterator it){};
};

TEST(_15_3_1_Test,test_MyList){
    MyList<int32_t> a;
    a.push_front(1);
    a.push_front(2);
    a.push_front(3);
    for(auto it =a.begin();it!=a.end();++it){
        std::cout<<*it<<std::endl;
    }
    for(auto& e:a){
        std::cout<<e<<std::endl;
    }
    int32_t ha = *high(std::begin(a), std::end(a));
    std::cout << "int32_t list's max is " << ha << std::endl;
}
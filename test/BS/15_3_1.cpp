#include "gtest/gtest.h"
#include <iostream>
#include <type_traits>


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

template <typename It>
auto high_value(It b,It e)->
typename std::remove_reference<decltype(*b)>::type {

    It ret=b;
    for(It i=b;i!=e;++i){
        if(*i>*ret){
            ret=i;
        }
    }
    return *ret;
} 





TEST(_15_3_1_Test,test_use){

   int32_t a[]{1,2,3};
   int32_t ha=high_value(std::begin(a),std::end(a));
   EXPECT_EQ(ha,3);

   std::vector<int32_t> b{1,2,3};
   int32_t hb=high_value(std::begin(b),std::end(b));
   EXPECT_EQ(hb,3);

   float fa[]{1,2,3};
   float hfa=high_value(std::begin(a),std::end(a));
   EXPECT_EQ(hfa,3);

   std::vector<float> fb{1,2,3};
   float hfb=high_value(std::begin(b),std::end(b));
   EXPECT_EQ(hfb,3);


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
//    for(auto it =a.begin();it!=a.end();++it){
//        std::cout<<*it<<std::endl;
//    }
//    for(auto& e:a){
//        std::cout<<e<<std::endl;
//    }
    int32_t ha = high_value(std::begin(a), std::end(a));
    EXPECT_EQ(ha,3);
}
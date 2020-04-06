
#include "gmock/gmock.h"
#include <any>

namespace observer
{


    template<typename T>
    class Observer;


    template<typename T>
    class Observable
    {
        public:
        void subscript(Observer<T>* ob)
        {
            m_observers.push_back(ob);
        }
        void notify(T* source, std::string field_name, std::any new_value) ;

    private:
        std::vector<Observer<T> *> m_observers;
    };

    class Person : public Observable<Person>
    {
    public:
        void set_age(int age);
    private:
        int m_age = 0;
    };

    template <typename T>
    class Observer
    {
        public:
        virtual void field_changed(T* source,std::string field_name,std::any new_value) {};
    };

    class Person_Observer_Mock:public Observer<Person>
    {
        public:
        MOCK_METHOD(void,field_changed,(Person* source,std::string field_name,std::any new_value),(override));
    };


    void Person::set_age(int age)
    {
        m_age = age;
        Observable<Person>::notify(this,"age",age);
    }

    template <typename T>
    void Observable<T>::notify(T *source, std::string field_name, std::any new_value)
    {
        for (auto ob : m_observers)
        {
            ob->field_changed(source, field_name, new_value);
        }
    }


} // namespace observer

using namespace testing;
using namespace observer;

TEST(Observer_Test, test_set_will_trigger_call)
{
    Person p;
    Person_Observer_Mock ob0;
    Person_Observer_Mock ob1;
    p.subscript(&ob0);
    p.subscript(&ob1);

    EXPECT_CALL(ob0, field_changed(&p,StrEq("age"),_))
    .Times(1);
    EXPECT_CALL(ob1, field_changed(&p,StrEq("age"),_))
    .Times(1);

    p.set_age(33);
}
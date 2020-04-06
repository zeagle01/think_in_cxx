
#include "gmock/gmock.h"

namespace observer
{


    template<typename T>
    class Observer;

    class Person
    {
    public:
        void subscript(Observer<Person> *ob);
    };

    template <typename T>
    class Observer
    {
        public:
        virtual void change_recieved() = 0;
    };

    class Person_Observer_Mock:public Observer<Person>
    {
        public:
        MOCK_METHOD(void,change_recieved,(),(override));
    };

    void Person::subscript(Observer<Person> *ob)
    {
        ob->change_recieved();
    }

} // namespace observer

using namespace testing;
using namespace observer;

TEST(Observer_Test, test)
{
    Person p;
    Person_Observer_Mock ob0;
    Person_Observer_Mock ob1;
    EXPECT_CALL(ob0, change_recieved);
    EXPECT_CALL(ob1, change_recieved);

    p.subscript(&ob0);
    p.subscript(&ob1);
}
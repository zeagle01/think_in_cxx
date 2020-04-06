

#include "gmock/gmock.h"
#include <string>
#include <memory>



namespace type_erasure
{


//given some classed that have same "get_name" method
    class A
    {
        public:
        std::string get_name()
        {
            return "A";
        }

        std::string say_A()
        {
            return "I'm in A";

        } 
    };

    class B
    {
        public:
        std::string get_name()
        {
            return "B";
        }

        std::string say_A()
        {
            return "I'm in B";

        } 
    };

    class C
    {
        public:
        std::string get_name()
        {
            return "C";
        }

        std::string say_A()
        {
            return "I'm in C";

        } 
    };

    class Type_Erasure
    {
        class Concept;
        template <typename T>
        class Model;

        using concept_ptr = std::shared_ptr<Concept>;

    public:
        template <typename T>
        Type_Erasure(T *a) : m_concept(std::make_shared <Model<T>>(a)) {}

        std::string get_name()
        {
            return m_concept->get_name();
        }

    private:
        class Concept
        {
        public:
            virtual std::string get_name() = 0;
        };

        template <typename T>
        class Model : public Concept
        {

        public:
            Model(T *obj) : m_obj(obj) {}
            virtual std::string get_name()
            {
                return m_obj->get_name();
            }

        private:
            T *m_obj;
        };

        concept_ptr m_concept;
    };
}



using namespace testing;

using namespace type_erasure;





TEST(Type_Erasure_Test,test_class_A_say_A)
{
    type_erasure::A a;
    EXPECT_THAT(a.say_A(),StrEq("I'm in A"));
}

TEST(Type_Erasure_Test,test_A_B_C_in_same_container)
{
    std::vector<Type_Erasure> named_list;
    type_erasure::A a;
    type_erasure::B b;
    type_erasure::C c;
    named_list.emplace_back(&a);
    named_list.emplace_back(&b);
    named_list.emplace_back(&c);

    EXPECT_THAT(named_list[0].get_name(),StrEq("A"));
    EXPECT_THAT(named_list[1].get_name(),StrEq("B"));
    EXPECT_THAT(named_list[2].get_name(),StrEq("C"));
}
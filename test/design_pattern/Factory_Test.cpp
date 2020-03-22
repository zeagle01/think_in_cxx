

#include "gmock/gmock.h"
#include <string>
#include <map>




#define REGISTER_FACTORY(derivedClass) \
    namespace {auto registry_ ## derivedClass =factory_test::ConcreteFactory<factory_test::derivedClass>(#derivedClass); }

namespace factory_test
{


    class Base
    {
        public:
        virtual std::string get_type()
        {
            return "Base";
        };

    };


    class DerivedA:public Base
    {
        public:
        virtual std::string get_type() override
        {
            return "DerivedA";
        };

    };

    class DerivedB:public Base
    {
        public:
        virtual std::string get_type() override
        {
            return "DerivedB";
        };

    };

    class Factory
    {
    private:
        using Class_Map = std::map<std::string, Factory *>;
        static Class_Map &get_class_map()
        {
            static Class_Map class_map{};
            return class_map;
        }

    public:
        static void registerFactory(const std::string & name,Factory* factory)
        {
            auto &class_map=get_class_map();
            class_map[name]=factory;
        }

        static Base *make_raw(const std::string &name)
        {
            auto class_map=get_class_map();
            if(class_map.count(name))
            {
                return class_map[name]->construct();
            }
            return nullptr;

        }

        static std::unique_ptr<Base> make_unique(const std::string& name)
        {
            return std::unique_ptr<Base>(make_raw(name));
        }
        //static make
        virtual Base* construct()=0;
    };

    template <typename DerivedClass>
    class ConcreteFactory : Factory
    {
    public:
        ConcreteFactory(const std::string &name)
        {
            Factory::registerFactory(name,this);
        }

        virtual Base *construct()
        {
            return new DerivedClass;
        }
    };

REGISTER_FACTORY(Base)
REGISTER_FACTORY(DerivedA)
REGISTER_FACTORY(DerivedB)

}



using namespace testing;

TEST(Factory_Test,test_get_base_type)
{
    auto cc=factory_test::Factory::make_unique("Base");
    EXPECT_THAT(cc->get_type(),Eq("Base"));

}


TEST(Factory_Test,test_get_derived_type)
{
    auto cc=factory_test::Factory::make_unique("DerivedA");
    EXPECT_THAT(cc->get_type(),Eq("DerivedA"));

}



#include "gmock/gmock.h"


#include <variant>
#include <vector>

using namespace testing;

namespace cxx_variant
{

class A
{
public:
    void do_something() 
    {
        printf("in A::do_something\n");
    }
};

class B
{
public:
    void do_something() 
    {
        printf("in B::do_something\n");
    }
};

} // namespace cxx_variant

TEST(Cxx_Variant,test_duck)
{
    using Doable=std::variant<cxx_variant::A,cxx_variant::B>;
    std::vector<Doable> doables;
    doables.push_back(cxx_variant::A());
    doables.push_back(cxx_variant::B());
    auto doable_visitor=[]( auto& doable){doable.do_something();};
    for(size_t i=0;i<doables.size();i++)
    {
        std::visit(doable_visitor,doables[i]);
    }
}
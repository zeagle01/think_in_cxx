


#include "gmock/gmock.h"
#include <memory>

#include <string>



namespace open_closed_principle{
    enum class Color{Red,Green,Blue};
    enum class Size{Large,Medium,Small};
    struct Product
    {
        std::string name;
        Color color;
        Size size;
    };

    template <typename Item>
    class Specification
    {
    public:
        virtual bool is_satisfied(Item *p) = 0;
    };

    class By_Color:public Specification<Product>
    {
        public:
        By_Color(Color color) : m_color(color) {}
        virtual bool is_satisfied(Product *p)  override
        {
            if (p->color == m_color)
            {
                return true;
            }else 
            {
                return false;
            }

        }
        private:
        Color m_color;
    };

    template <typename Item>
    class Filter
    {
        public:
        std::vector<Item*> filter(const std::vector<Item*>& products,Specification<Item>& specification)
        {
            std::vector<Item *> ret;
            for(auto it: products)
            {
                if (specification.is_satisfied(it))
                {
                    ret.push_back(it);
                }
            }
            return ret;
        }
    };

}
using namespace open_closed_principle;
using namespace testing;

class Some_Products_Filter_Test:public testing::Test
{

    protected:
    virtual void SetUp()
    {
        p0 = {"rl", Color::Red, Size::Large};
        p1 = {"gm", Color::Green, Size::Medium};
        p2 = {"bs", Color::Blue, Size::Small};
        products.push_back(&p0);
        products.push_back(&p1);
        products.push_back(&p2);

    }

    std::vector<Product*> products;
    Product p0;
    Product p1;
    Product p2;
    
};



TEST_F(Some_Products_Filter_Test,test_init_poducts_size_is_three)
{
    EXPECT_THAT(products.size(),Eq(3));
}



TEST_F(Some_Products_Filter_Test,by_color)
{
    Filter<Product> f;
    By_Color by_color_is_red(Color::Red);

    auto ret = f.filter(products,by_color_is_red);
    for(auto it:ret)
    {
        EXPECT_THAT(it->color, Eq(Color::Red));
    }
}

TEST_F(Some_Products_Filter_Test,by_size)
{
    //TODO
//    Filter f;
//    Specification by_size;
//    f.filter(by_size,products);
//    auto ret = f.by_color(products,Color::Red);
//    for(auto it:ret)
//    {
//        EXPECT_THAT(it->color, Eq(Color::Red));
//    }
}
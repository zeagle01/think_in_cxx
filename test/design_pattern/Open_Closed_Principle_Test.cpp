


#include "gmock/gmock.h"
#include <memory>

#include <string>



namespace open_closed_principle{
    /////////////////// data
    enum class Color{Red,Green,Blue};
    enum class Size{Large,Medium,Small};
    struct Product
    {
        std::string name;
        Color color;
        Size size;
    };


    /////////////////// specification
    template <typename Item>
    class And_Spec;

    template <typename Item>
    class Specification
    {
    public:
        virtual bool is_satisfied(Item *p) = 0;
        And_Spec<Item> operator&&(Specification<Item> &other );
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

    class By_Size:public Specification<Product>
    {
        public:
        By_Size(Size size) : m_size(size) {}
        virtual bool is_satisfied(Product *p)  override
        {
            if (p->size == m_size)
            {
                return true;
            }else 
            {
                return false;
            }

        }
        private:
        Size m_size;
    };

    template <typename Item>
    class And_Spec : public Specification<Item>
    {
        public:
            And_Spec(Specification<Item> &left, Specification<Item> &right) :m_left(left),m_right(right){}

        virtual bool is_satisfied(Item *p)  override
        {
            if(m_left.is_satisfied(p)&&m_right.is_satisfied(p))
            {
                return true;
            }else
            {
                return false;
            }
        }
        private:
            Specification<Item>& m_left;
            Specification<Item>& m_right;
    };

    template <typename Item>
    And_Spec<Item> Specification<Item>::operator&&(Specification<Item> &other)
    {
        return And_Spec<Item>(*this,other);
    }


    /////////////////// filter
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

    Filter<Product> f;
    
};



TEST_F(Some_Products_Filter_Test,test_init_poducts_size_is_three)
{
    EXPECT_THAT(products.size(),Eq(3));
}

TEST_F(Some_Products_Filter_Test,by_color)
{
    By_Color by_color_is_red(Color::Red);
    auto ret = f.filter(products,by_color_is_red);
    EXPECT_THAT(ret[0]->name, StrEq("rl"));
}

TEST_F(Some_Products_Filter_Test,by_size)
{
    By_Size by_size(Size::Small);
    auto ret = f.filter(products,by_size);
    EXPECT_THAT(ret[0]->name, StrEq("bs"));
}

TEST_F(Some_Products_Filter_Test,by_size_and_color)
{
    By_Size by_size(Size::Small);
    By_Color by_color(Color::Blue);
    auto ret = f.filter(products, by_color && by_size);
    EXPECT_THAT(ret[0]->name, StrEq("bs"));
}

#include "gmock/gmock.h" 


using namespace testing;

template<typename ...T>
struct Type_List{};

template<typename Type_List>
struct Get_Front_Imp;

template<typename Head,typename ...Tails>
struct Get_Front_Imp<Type_List<Head, Tails...>>
{
	using type = Head;
};
template<typename Type_List>
using Get_Front = typename Get_Front_Imp<Type_List>::type;


template <typename Type_List> 
struct Pop_Front_Imp;

template <typename Head,typename ...Tails> 
struct Pop_Front_Imp < Type_List<Head, Tails...>>
{
	using type = Type_List<Tails...>;
};

template<typename Type_List>
using Pop_Front = typename Pop_Front_Imp<Type_List>::type;



TEST(Type_List_Test, get_front)
{
	using my_list = Type_List<float, int>;
	bool is_front_float = std::is_same_v<Get_Front<my_list>, float>;
	EXPECT_TRUE(is_front_float);

}

TEST(Type_List_Test, pop_type_from_list)
{
	bool is_type_list_same = std::is_same_v<Pop_Front<Type_List<float, int>>, Type_List< int>>;
	EXPECT_TRUE(is_type_list_same);

}

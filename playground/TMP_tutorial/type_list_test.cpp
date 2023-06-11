
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


template<typename Type, typename Type_List>
struct Push_Front_Imp;

template<typename Type,typename ...Tails>
struct Push_Front_Imp<Type, Type_List< Tails...>>
{
	using type = Type_List<Type, Tails...>;
};

template<typename Type,typename Type_List>
using Push_Front = Push_Front_Imp<Type, Type_List>::type;



template<typename Type_List>
struct For_Each_Types;

template<typename ...Types>
struct For_Each_Types<Type_List<Types...>>
{
	template<typename Functor, typename ...Param>
	static void Apply(Param&& ... param)
	{
		(typename Functor::template Apply<Types>(std::forward<Param>(param)...), ...);
	}
};


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

TEST(Type_List_Test, push_front_to_type_list)
{
	bool is_type_list_same = std::is_same_v<Push_Front<float, Type_List< int>>, Type_List<float, int>>;
	EXPECT_TRUE(is_type_list_same);

}


struct Get_Type_Size
{
	template<typename T>
	static void Apply(std::vector<int>& sizes)
	{
		sizes.push_back(sizeof(T));
	}
};

TEST(Type_List_Test, for_each_types_get_size)
{
	std::vector<int> sizes;
	For_Each_Types<Type_List<float, char>>::Apply<Get_Type_Size>(sizes);
	EXPECT_THAT(sizes, Eq(std::vector<int>{4, 1}));

}


struct My_Type
{
	int a = 1;
	float b = 2.f;
};


TEST(Type_List_Test, structured_binding)
{
	My_Type data;
	auto [x, y] = data;
	EXPECT_THAT(x, Eq(1));
	EXPECT_THAT(y, Eq(2.f));

}

template<typename Tuple>
struct Extract_Type_List_Imp;

template<typename ...Type>
struct Extract_Type_List_Imp<std::tuple<Type...>>
{
	using type = Type_List<std::decay_t<Type>...>;
};

template<typename Tuple>
using Extract_Type_List = typename Extract_Type_List_Imp<Tuple>::type;

TEST(Type_List_Test, get_type_list_from_tuple)
{
	My_Type data;
	auto [x, y] = data;
	using my_type_list =  Extract_Type_List<decltype(std::tie(x, y))>;
	bool is_type_list_same = std::is_same_v<my_type_list, Type_List<int, float>>;
	EXPECT_TRUE(is_type_list_same);

}

//get field count
struct To_Any
{
	template<typename T>
	operator T();
};

template<typename T,  typename ...P>
struct Get_Field_Count_Imp
{
	static constexpr int value = sizeof...(P) - 1;
};

template<typename T, typename ...P>
requires  requires{T{ P{}... }; }
struct Get_Field_Count_Imp < T,  P... >
{
	static constexpr int value = Get_Field_Count_Imp<T, To_Any, P...>::value;
};

template<typename L>
inline constexpr int Field_Count = Get_Field_Count_Imp<L>::value;

TEST(Type_List_Test, get_field_count)
{
	//constexpr int field_count = Field_Count<My_Type>;
	EXPECT_THAT(Field_Count<My_Type>, Eq(2));
}

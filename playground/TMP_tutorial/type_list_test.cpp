
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
	using type = Type_List<std::remove_pointer_t<std::decay_t<Type>>...>;
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
	template<typename T> operator T();
	template<typename T> operator T*();
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


template<typename T>
constexpr auto As_Tuple()
{
	if constexpr (Field_Count<T> == 1)
	{
		auto [v0] = T{};
		return std::tie(v0);
	}
	else if constexpr (Field_Count<T> == 2)
	{
		auto [v0, v1] = T{};
		return std::tie(v0,v1);
	}
	//more else if clause ...
}


TEST(Type_List_Test, get_type_list_with_computed_field_count)
{
	using my_type_list =  Extract_Type_List<decltype(As_Tuple<My_Type>())>;
	bool is_type_list_same = std::is_same_v<my_type_list, Type_List<int, float>>;
	EXPECT_TRUE(is_type_list_same);

}



class Type_Map
{
public:
	template<typename VarName>
	auto& GetRef()
	{
		auto key = std::string(typeid(VarName).name());
		return *std::static_pointer_cast<typename VarName::type>(m_datas[key]);
	}

	template<typename VarName>
	const auto& GetRef() const 
	{
		auto key = std::string(typeid(VarName).name());
		return  *std::static_pointer_cast<typename VarName::type>(m_datas[key]);
	}

	template<typename VarGroup>
	void Build()
	{
		using types = Extract_Type_List<decltype(As_Tuple<VarGroup>())>;
		For_Each_Types<types>::template Apply<AddVar>(m_datas);
	}

private:
	using map_t = std::map<std::string, std::shared_ptr<void>>;
	map_t m_datas;

private:
	struct AddVar
	{
		template<typename VarName>
		static void Apply(map_t& datas)
		{
			auto key = std::string(typeid(VarName).name());
			datas[key] = std::make_shared<typename VarName::type>();
		}

	};
};


struct Config_List
{
	struct A { using type = int; } *a_var;
	struct B { using type = float; } *b_var;
};


TEST(Type_List_Test, My_Type_Map)
{
	Type_Map datas;
	datas.Build<Config_List>();
	datas.GetRef<Config_List::A>() = 42;
	datas.GetRef<Config_List::B>() = 2.f;
	EXPECT_THAT(datas.GetRef<Config_List::A>(), Eq(42));
	EXPECT_THAT(datas.GetRef<Config_List::B>(), Eq(2.f));

}

#define ADD_VARIABLE(name,t,v)  struct name { using type = t; static constexpr type default_value = v; }; name* name##_var; 

struct Variable_With_Default_Value
{
	ADD_VARIABLE(A, int, 42);
	ADD_VARIABLE(B, float, 2.f);
};

struct Reset_Value
{
	template<typename VarName>
	static void Apply(Type_Map& data)
	{
		data.GetRef<VarName>() = VarName::default_value;
	}
};


TEST(Type_List_Test, reset_all_values_for_type_map)
{
	Type_Map datas;
	datas.Build<Variable_With_Default_Value>();
	datas.GetRef<Variable_With_Default_Value::A>() = 0;
	datas.GetRef<Variable_With_Default_Value::B>() = 0.f;

	using variables = Extract_Type_List<decltype(As_Tuple<Variable_With_Default_Value>())>;
	For_Each_Types<variables>::template Apply<Reset_Value>(datas);

	EXPECT_THAT(datas.GetRef<Variable_With_Default_Value::A>(), Eq(42));
	EXPECT_THAT(datas.GetRef<Variable_With_Default_Value::B>(), Eq(2.f));

}


template<typename T>
struct Atomic_Data
{

	//thread 1
	void Set(const T& v)
	{
		std::lock_guard lock(m_mutex);
		m_value = v;
		m_is_dirty = true;
	}

	//thread 2
	void CopyToCache()
	{
		std::lock_guard lock(m_mutex);
		if (m_is_dirty)
		{
			m_cache = m_value;
		}
	}

	//thread 2
	bool IsChanged()
	{
		std::lock_guard lock(m_mutex);
		return m_is_dirty;
	}

	//thread 2
	const T& GetCache() const
	{
		std::lock_guard lock(m_mutex);
		return m_cache;
	}

	//thread 2
	void ResetChange()
	{
		std::lock_guard lock(m_mutex);
		m_is_dirty = false;
	}
private:
	mutable std::mutex m_mutex;
	bool m_is_dirty = true;
	T m_value;
	T m_cache;
};


#define ADD_ATOMIC_DATA(name,t)  struct name { using type = Atomic_Data<t>;}; name* name##_var; 

struct Atomic_Datas
{
	struct VarList
	{
		ADD_ATOMIC_DATA(A, int);
		ADD_ATOMIC_DATA(B, float);
	};

	Atomic_Datas() { m_datas.Build<VarList>(); }

	template<typename VarName,typename T>
	void Set(const T& v) { m_datas.GetRef<VarName>().Set( v); }

	template<typename VarName>
	const auto& GetCache() { return m_datas.GetRef<VarName>().GetCache(); }

	template<typename VarName>
	bool IsChanged() { return m_datas.GetRef<VarName>().IsChanged(); }

	template<typename VarName>
	void ResetChange() { m_datas.GetRef<VarName>().ResetChange(); }

	template<typename VarName>
	void CopyToCache() { m_datas.GetRef<VarName>().CopyToCache(); }

private:
	Type_Map m_datas;
};


struct Capture
{
	template<typename VarName>
	static void Apply(Atomic_Datas& data)
	{
		data.CopyToCache<VarName>();
	}
};

struct ResetChanges
{
	template<typename VarName>
	static void Apply(Atomic_Datas& data)
	{
		data.template ResetChange<VarName>();
	}
};


TEST(Type_List_Test, atomic_data_captures)
{
	Atomic_Datas datas;
	//thread 1
	datas.Set<Atomic_Datas::VarList::A>(42);
	datas.Set<Atomic_Datas::VarList::B>(2.f);

	//thread 2 from now on

	//capture all datas
	using variables = Extract_Type_List<decltype(As_Tuple<Atomic_Datas::VarList>())>;
	For_Each_Types<variables>::template Apply<Capture>(datas);

	if(datas.IsChanged<Atomic_Datas::VarList::A>())
	{
		//do some computations
	}

	EXPECT_THAT(datas.GetCache<Atomic_Datas::VarList::A>(), Eq(42));
	EXPECT_THAT(datas.GetCache<Atomic_Datas::VarList::B>(), Eq(2.f));

	using variables = Extract_Type_List<decltype(As_Tuple<Atomic_Datas::VarList>())>;
	For_Each_Types<variables>::template Apply<ResetChanges>(datas);

	EXPECT_FALSE(datas.IsChanged<Atomic_Datas::VarList::A>());
	EXPECT_FALSE(datas.IsChanged<Atomic_Datas::VarList::B>());

}

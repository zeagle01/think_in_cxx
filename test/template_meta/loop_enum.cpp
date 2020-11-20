
#include "gmock/gmock.h"
#include  <type_traits>
#include <string>


namespace loop_enum
{

	enum class My_Cat
	{
		A,
		B,
		C
	};


	template<My_Cat c>
	struct mc_traits;

	template<>
	struct mc_traits<My_Cat::A>
	{
		using type = int;
	};

	template<>
	struct mc_traits<My_Cat::B>
	{
		using type = float;
	};

	template<>
	struct mc_traits<My_Cat::C>
	{
		using type = double;
	};




	template<typename E,E V>
	constexpr bool is_valid()
	{
		auto name = __FUNCSIG__;
		int i = 0;
		for(;;i++)
		{
			if (name[i] == '>')
			{
				i--;
				break;
			}
			if (name[i] == '\0')
			{
				return false;
			}

		}


		if (name[i] >= '0' && name[i] <= '9')
		{
			for (;i>=0; i--)
			{
				if (name[i] == ')') {
					break;
				}
			}
			if (name[i+1] == '0' && name[i+2] == 'x')
			{
				return false;
			}
		}

		return true;
	};



	template<typename E, int h = 0 >
	struct loop_enum
	{
		template<template<typename U, U u>  typename F,typename ...P>
		static void apply(P&&... p)
		{
			if constexpr (is_valid<E, E(h)>())
			{
				F<E, E(h)>::apply(std::forward<P>(p)...);
				loop_enum<E, h + 1>::template apply<F>(std::forward<P>(p)...);
			}
		}

	};


	template<typename E,E v>
	struct process
	{
		static void apply(std::vector<std::string>& strings) 
		{
			strings.push_back(typeid(mc_traits<v>::type).name());
		}
	};


	TEST(TEST_Loop_Enum, is_valid)
	{
		auto act = is_valid<My_Cat, My_Cat(3)>();
		EXPECT_FALSE(act);
		act = is_valid<My_Cat, My_Cat(2)>();
		EXPECT_TRUE(act);
	}

	TEST(TEST_Loop_Enum, collect)
	{
		std::vector<std::string> act;

		

		loop_enum<My_Cat>::apply<process>(act);


		std::vector<int> exp{0,1,2};

	}

	////////////////////////////////

	enum class Shader_Type
	{
		Int,
		Int2,
		Int3,
		Float,
		Float2,
		Float3
	};

	template<typename T,int N>
	struct data_info
	{
		using type = T;
		static constexpr int count = N;
		static constexpr int size = count * sizeof(T);

	};

	template<Shader_Type c>
	struct shader_type_traits;
	template<> struct shader_type_traits<Shader_Type::Int> :public data_info<int, 1> { };
	template<>struct shader_type_traits<Shader_Type::Int2> :public data_info<int, 2> { };
	template<>struct shader_type_traits<Shader_Type::Int3> :public data_info<int, 3> { };
	template<> struct shader_type_traits<Shader_Type::Float> :public data_info<float, 1> { };
	template<>struct shader_type_traits<Shader_Type::Float2> :public data_info<float, 2> { };
	template<>struct shader_type_traits<Shader_Type::Float3> :public data_info<float, 3> { };


	struct Layout
	{
		Shader_Type st;
		int count;
		int size;
	};

	template<typename E,E v>
	struct collect_layouts
	{
		static void apply(std::vector<Layout>& layout) 
		{
			layout.push_back({ v,shader_type_traits<v>::count,shader_type_traits<v>::size });
		}
	};

	TEST(Test_Loop_Enum, gl_types)
	{
		std::vector<Layout> act;
		loop_enum<Shader_Type>::apply<collect_layouts>(act);

	}

}
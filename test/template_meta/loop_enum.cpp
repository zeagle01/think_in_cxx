
#include "gmock/gmock.h"
#include  <type_traits>
#include <string>

using namespace testing;

namespace loop_enum
{

	enum class My_Cat
	{
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		Mat3,
		Mat4
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
	template<>
	struct mc_traits<My_Cat::D>
	{
		using type = int;
	};


	template<>
	struct mc_traits<My_Cat::E>
	{
		using type = double;
	};
	template<>
	struct mc_traits<My_Cat::F>
	{
		using type = int;
	};

	template<>
	struct mc_traits<My_Cat::G>
	{
		using type = float;
	};

	template<>
	struct mc_traits<My_Cat::H>
	{
		using type = double;
	};
	template<>
	struct mc_traits<My_Cat::I>
	{
		using type = float;
	};

	template<>
	struct mc_traits<My_Cat::Mat3>
	{
		using type = double;
	};

	template<>
	struct mc_traits<My_Cat::Mat4>
	{
		using type = double;
	};



	template<typename E,E V>
	constexpr void get_enum_sig(std::vector<std::string>& enum_names)
	{
		enum_names.push_back(__FUNCSIG__);
	}


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


		for (; i >= 0; i--)
		{
			if (name[i] == ')') {
				break;
			}
		}
		if (i == -1)
		{
			return true;
		}

		if (name[i + 1] == '0' && name[i + 2] == 'x')
		{
			return false;
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

	template<typename E, int h = 0 >
	struct loop_enum_debug
	{
		static void apply(std::vector < std::string>& enum_names )
		{
			get_enum_sig<E, E(h)>(enum_names);
			if constexpr ( is_valid<E, E(h)>() )
			{
				loop_enum_debug<E, h + 1>::apply(enum_names);
			}
		}

	};


	template<typename E,E v>
	struct process
	{
		static void apply(std::vector<std::string>& strings) 
		{
			//strings.push_back(typeid(mc_traits<v>::type).name());
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
		std::vector<std::string> debug_info;

		

		//loop_enum<My_Cat>::apply<process>(act);
		loop_enum_debug<My_Cat>::apply(debug_info);


		std::vector<int> exp{0,1,2};

	}

	////////////////////////////////

	enum class Shader_Type
	{
		Bool,
		Int,
		Int2,
		Int3,
		Int4,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4

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

#define DEF_Shader_Type_Record(x,t,c) \
	template<> struct shader_type_traits<Shader_Type::x> :public data_info<t, c> { };

DEF_Shader_Type_Record(Bool,bool,1)
DEF_Shader_Type_Record(Int,int,1)
DEF_Shader_Type_Record(Int2,int,2)
DEF_Shader_Type_Record(Int3,int,3)
DEF_Shader_Type_Record(Int4,int,4)
DEF_Shader_Type_Record(Float,float,1)
DEF_Shader_Type_Record(Float2,float,2)
DEF_Shader_Type_Record(Float3,float,3)
DEF_Shader_Type_Record(Float4,float,4)
DEF_Shader_Type_Record(Mat3,float,3*3)
DEF_Shader_Type_Record(Mat4,float,4*4)


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

		std::vector<Layout> exp =
		{
			{Shader_Type::Bool,1, sizeof(bool) },
			{ Shader_Type::Int,1,sizeof(int) },
			{ Shader_Type::Int2,2,sizeof(int) },
			{ Shader_Type::Int3,3 ,sizeof(int)},
			{ Shader_Type::Int4,4 ,sizeof(int)},
			{ Shader_Type::Float,1 ,sizeof(float)},
			{ Shader_Type::Float2,2 ,sizeof(float)},
			{ Shader_Type::Float3,3 ,sizeof(float)},
			{ Shader_Type::Float4,4 ,sizeof(float)},
			{ Shader_Type::Mat3,3 * 3 ,sizeof(float)},
			{ Shader_Type::Mat4 , 4 * 4,sizeof(float) }
		};

		//EXPECT_THAT(act, Eq(exp));

	}

}
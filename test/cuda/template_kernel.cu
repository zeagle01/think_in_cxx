
#include "gmock/gmock.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"


namespace template_kernel
{

	template<typename T,int N>
	struct H_Vec 
	{
		T data[N];
	};

	template<typename T,int N>
	struct D_Vec 
	{
		T data[N];
	};




	template<typename T, int N, int Plat_Form>
	struct Vec;



	template<int ... N>
	static __device__ auto sum()
	{
		return sizeof...(N); //variadic template
	}



	template<template<typename, int > typename Vec_Type, typename T, int N> //template template
	__global__ void vec_add(Vec_Type<T, N>* v)
	{
		printf("hello %d\n", sum<1, 2, 3>());
	}



	TEST(Template_Kernel_Test,vector_add)
	{

		D_Vec<float,3> d;
		vec_add << <1, 1 >> > (&d);
	}

}
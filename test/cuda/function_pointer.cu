
#include "gmock/gmock.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <vector>

using namespace testing;

namespace cuda_function_pointer
{

	template<typename T>
	using kernel_func_t = void (*)(T* a, T* b, int tid);

	template<typename T>
	__global__ void cuda_loop(kernel_func_t<T> kernel_func, T* a, T* b, int n)
	{
		int tid = threadIdx.x + blockDim.x * blockIdx.x;
		if (tid < n)
		{
			kernel_func(a, b, tid);
		}

	}

	template<typename T>
	__device__ void compute_kernel( T* a, T* b, int tid)
	{
		a[tid] = b[tid] * b[tid];
	}

	template<typename T>
	__device__ void compute_kernel_1( T* a, T* b, int tid)
	{
		a[tid] = b[tid] * b[tid] * b[tid];
	}

	template<typename T>
	__device__ kernel_func_t<T> d_pointer_compute_kernel = compute_kernel<T>;

	template<typename T>
	__device__ kernel_func_t<T> d_pointer_compute_kernel_1 = compute_kernel_1<T>;

	TEST(Cuda_Function_Pointer,use)
	{
		int num = 100;
		std::vector<float> h_a(num);
		std::vector<float> h_b(num);
		for (int i = 0; i < num; i++)
		{
			h_b[i] = i;
		}

		float* d_a;
		float* d_b;
		size_t size_in_byte = sizeof(float) * num;
		cudaMalloc((void**)&d_a, size_in_byte);
		cudaMalloc((void**)&d_b, size_in_byte);
		cudaMemcpy(d_a, h_a.data(), size_in_byte, cudaMemcpyHostToDevice);
		cudaMemcpy(d_b, h_b.data(), size_in_byte, cudaMemcpyHostToDevice);

		kernel_func_t<float> h_kernel_func;

		{
			cudaMemcpyFromSymbol(&h_kernel_func, d_pointer_compute_kernel<float>, sizeof(kernel_func_t<float>));

			int tpb = 128;
			int bpg = (num + tpb - 1) / tpb;
			cuda_loop << <bpg, tpb >> > (h_kernel_func, d_a, d_b, num);

			cudaMemcpy(&h_a[0], d_a, size_in_byte, cudaMemcpyDeviceToHost);
			for (int i = 0; i < num; i++)
			{
				EXPECT_THAT(h_a[i], Eq(i * i));
			}
		}

		{
			cudaMemcpyFromSymbol(&h_kernel_func, d_pointer_compute_kernel_1<float>, sizeof(kernel_func_t<float>));

			int tpb = 128;
			int bpg = (num + tpb - 1) / tpb;
			cuda_loop << <bpg, tpb >> > (h_kernel_func, d_a, d_b, num);

			cudaMemcpy(&h_a[0], d_a, size_in_byte, cudaMemcpyDeviceToHost);
			for (int i = 0; i < num; i++)
			{
				EXPECT_THAT(h_a[i], Eq(i * i * i));
			}
		}

	}


}
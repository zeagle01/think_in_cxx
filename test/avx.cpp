//#include "stdafx.h"

/*
#include "glm\glm.hpp"
#include <tbb\scalable_allocator.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "common\tinyobjloader-master\tiny_obj_loader.h"
*/

#ifndef USE_BT_HEADER
#include <fstream>
#include <vector>
#include <valarray>
#include <chrono>
#include <algorithm>
#include <random>
#include <numeric>
#include <xmmintrin.h>
#include <immintrin.h>
#include <nmmintrin.h>
#include <emmintrin.h>
#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
#endif // !USE_BT_HEADER




typedef float scalar;
typedef float Scalar;

const int rep = 5;

std::string addcommas(int i)
{
	std::string s;
	std::stringstream ss;
	std::locale mylocale("");
	ss.imbue(mylocale);
	ss << i;
	ss >> s;
	return s;
}

void bbsimd2(const float f[], int n, float bb3[])
{
	__m256 mi1 = _mm256_set1_ps(std::numeric_limits<float>::max());
	__m256 mi2 = mi1;
	__m256 mi3 = mi1;
	__m256 ma1 = _mm256_set1_ps(-std::numeric_limits<float>::max());
	__m256 ma2 = ma1;
	__m256 ma3 = ma1;

	for (int i = 0; i < n; i += 8)
	{
		__m256 pos1 = _mm256_loadu_ps(f + i * 3 + 0);
		__m256 pos2 = _mm256_loadu_ps(f + i * 3 + 8);
		__m256 pos3 = _mm256_loadu_ps(f + i * 3 + 16);

		mi1 = _mm256_min_ps(pos1, mi1);
		mi2 = _mm256_min_ps(pos2, mi2);
		mi3 = _mm256_min_ps(pos3, mi3);

		ma1 = _mm256_max_ps(pos1, ma1);
		ma2 = _mm256_max_ps(pos2, ma2);
		ma3 = _mm256_max_ps(pos3, ma3);
	}

	float temp_min1[24];
	float temp_max1[24];

	_mm256_storeu_ps(temp_min1, mi1);
	_mm256_storeu_ps(temp_max1, ma1);
	_mm256_storeu_ps(temp_min1 + 8, mi2);
	_mm256_storeu_ps(temp_max1 + 8, ma2);
	_mm256_storeu_ps(temp_min1 + 16, mi3);
	_mm256_storeu_ps(temp_max1 + 16, ma3);

	float x, y, z;
	x = y = z = -std::numeric_limits<float>::max();
	float xx, yy, zz;
	xx = yy = zz = std::numeric_limits<float>::max();

	for (int i = 0; i < 8; i++)
	{
		x = std::max(x, temp_max1[3 * i + 0]);
		y = std::max(y, temp_max1[3 * i + 1]);
		z = std::max(z, temp_max1[3 * i + 2]);

		xx = std::min(xx, temp_min1[3 * i + 0]);
		yy = std::min(yy, temp_min1[3 * i + 1]);
		zz = std::min(zz, temp_min1[3 * i + 2]);
	}

	bb3[0] = x;
	bb3[1] = y;
	bb3[2] = z;

	bb3[0 + 3] = xx;
	bb3[1 + 3] = yy;
	bb3[2 + 3] = zz;
}
void bbsimd(const float f[], int n, float bb3[])
{
	__m256 mi = _mm256_set1_ps(std::numeric_limits<float>::max());
	__m256 ma = _mm256_set1_ps(-std::numeric_limits<float>::max());
	int nn = n / 2;
	for (int i = 0; i < nn; i++)
	{
		__m256 pos = _mm256_loadu_ps(f + i * 6);
		mi = _mm256_min_ps(pos, mi);
		ma = _mm256_max_ps(pos, ma);

	}

	float temp1_min[8];
	float temp2_max[8];

	_mm256_storeu_ps(temp1_min, mi);
	_mm256_storeu_ps(temp2_max, ma);

	bb3[0] = std::max(temp2_max[0], temp2_max[3]);
	bb3[1] = std::max(temp2_max[1], temp2_max[4]);
	bb3[2] = std::max(temp2_max[2], temp2_max[5]);

	bb3[0 + 3] = std::min(temp1_min[0], temp1_min[3]);
	bb3[1 + 3] = std::min(temp1_min[1], temp1_min[4]);
	bb3[2 + 3] = std::min(temp1_min[2], temp1_min[5]);
}

void bb(const float f[], int n, float bb3[])
{
	float x, y, z;
	x = y = z = -std::numeric_limits<float>::max();
	float xx, yy, zz;
	xx = yy = zz = std::numeric_limits<float>::max();
	for (int i = 0; i < n; i++)
	{
		x = std::max(x, f[3 * i + 0]);
		y = std::max(y, f[3 * i + 1]);
		z = std::max(z, f[3 * i + 2]);

		xx = std::min(xx, f[3 * i + 0]);
		yy = std::min(yy, f[3 * i + 1]);
		zz = std::min(zz, f[3 * i + 2]);
	}
	bb3[0] = x;
	bb3[1] = y;
	bb3[2] = z;

	bb3[0 + 3] = xx;
	bb3[1 + 3] = yy;
	bb3[2 + 3] = zz;
}


float sm(float* f, int n)
{
	float s = 0;

	for (int i = 0; i < n; i++)
	{
		s += f[i];
	}

	return s;
}


float smsimd(float* f, int n)
{
	assert(n % 8 == 0);
	// set zero
	//__m256 temp_sum = _mm256_set1_ps(0);
	__m256 temp_sum = _mm256_setzero_ps();
	for (int i = 0; i < n; i += 8)
	{
		// load data
		__m256 ld = _mm256_loadu_ps(f + i);
		// add
		temp_sum = _mm256_add_ps(ld, temp_sum);
	}
	// temp_sum is 7 6 5 4 3 2 1 0

	// 3 2 1 0 7 6 5 4 
	__m256 swp = _mm256_permute2f128_ps(temp_sum, temp_sum, 1);
	// 73 62 51 40 73 62 51 40
	temp_sum = _mm256_add_ps(temp_sum, swp);
	// 7632 5410 7632 5410 7632 5410 7632 5410
	temp_sum = _mm256_hadd_ps(temp_sum, temp_sum);
	// 76543210 76543210 76543210 76543210 76543210 76543210 76543210 76543210
	temp_sum = _mm256_hadd_ps(temp_sum, temp_sum);

	float result;
	_mm_store_ss(&result, _mm256_castps256_ps128(temp_sum));
	//result = temp_sum.m256_f32[0];
	return result;
}



int lp(const float f[], float r[], int n, float limit = 0)
{
	float* cp = r;
	for (int i = 0; i < n; i++)
	{
		float temp = f[i];
		if (limit >= temp)
		{
			*cp = temp;
			cp++;
		}
	}

	return (int)(cp - r);
}

int lpsimd(const float f[], float r[], int n, float limit = 0)
{
	assert(n % 8 == 0);
	float* output = r;
	__m256 lm_simd = _mm256_set1_ps(limit);

	for (int i = 0; i < n; i += 8)
	{
		// load data
		__m256 ld = _mm256_loadu_ps(f + i);
		__m256 mask = _mm256_cmp_ps(lm_simd, ld, _CMP_GE_OQ);

		int move_mask = _mm256_movemask_ps(mask);                               // if we need index 0 and 2
																				// move_mask will be 0b 0000'0101
		{ // store filtered data
			uint64_t expanded_mask = _pdep_u64(move_mask, 0x0101010101010101);  // unpack each bit to a byte
																				// output will be  0b 0000'0001| 0000'0000 | 0000'0001

			expanded_mask *= 0xFF;                                              // mask |= mask<<1 | mask<<2 | ... | mask<<7;
																				// 0b ABC... -> 0b AAAA'AAAA'BBBB'BBBB'CCCC'CCCC...: 
																				// replicate each bit to fill its byte
																				// e.g we want index 0 2
																				// output will be  0b 1111'1111| 0000'0000 | 1111'1111

			const uint64_t identity_indices = 0x0706050403020100;    // the identity shuffle for vpermps, packed to one index per byte
			uint64_t wanted_indices = _pext_u64(identity_indices, expanded_mask); // e.g we want index 0 2
																				  // wanted_indices will be  0b000'...'0000 0010'0000 0000              
																				  //     [......,2,0]

			__m128i bytevec = _mm_cvtsi64_si128(wanted_indices);                  // add zeros at the beginning of wanted_indices, and it becomes __m128i
																				  // just a cast


			__m256i shufmask = _mm256_cvtepu8_epi32(bytevec);             // extand 8 packed 8bit int to 32bit
																		  // abc -> 000a 000b 000c

			__m256 newdata = _mm256_permutevar8x32_ps(ld, shufmask);      // shuffle unfiltered data across lane

			_mm256_storeu_ps(output, newdata);                            // store filtered data
		}
		// MOVE pointer
		output += _mm_popcnt_u64(move_mask);
	}


	return (int)(output - r);
}

void bbmain()
{
	const int vt = 400000;
	const int n = vt * 3;
	std::cout << std::endl;
	std::cout << "计算" << addcommas(vt) << "个三维顶点的AABB" << std::endl;


	struct r_num {
		int current;
		std::uniform_real_distribution<float> distribution[3];
		std::mt19937 mersenne_engine{  };
		r_num() {
			current = 0;
			distribution[0] = std::uniform_real_distribution<float>(-5, 2.83f);
			distribution[1] = std::uniform_real_distribution<float>(-16, 9);
			distribution[2] = std::uniform_real_distribution<float>(3.14f, 265.4f);
			std::random_device rnd_device;
			mersenne_engine = std::mt19937{ rnd_device() };
		}
		float operator()() {
			float r = distribution[current % 3](mersenne_engine);
			current++;
			return r;
		}
	} randNumber;


	std::vector<float> data(n + 18);

	std::generate(data.begin(), data.end(), randNumber);
	//std::iota(data.begin(), data.end(), 0);

	float bb3[6];
	float bbsimd3[6];
	float bbsimd33[6];
	bb(data.data(), vt, bb3);
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < rep; i++)
		bb(data.data(), vt, bb3);
	auto md = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < rep; i++)
		bbsimd(data.data(), vt, bbsimd3);
	auto ed = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < rep; i++)
		bbsimd2(data.data(), vt, bbsimd33);
	auto eded = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> d1 = (md - start);
	std::chrono::duration<double> d2 = (ed - md);
	std::chrono::duration<double> d3 = (eded - ed);

	std::cout << "scalar " << bb3[0] << " " << bb3[1] << " " << bb3[2] << " " << bb3[3] << " " << bb3[4] << " " << bb3[5] << "      " << d1.count() * 1000 / rep << "ms" << std::endl;
	std::cout << "simd1  " << bbsimd3[0] << " " << bbsimd3[1] << " " << bbsimd3[2] << " " << bbsimd3[3] << " " << bbsimd3[4] << " " << bbsimd3[5] << "      " << d2.count() * 1000 / rep << "ms" << std::endl;
	std::cout << "simd1 speed up " << d1.count() / d2.count() << "X" << std::endl;
	std::cout << "simd2  " << bbsimd33[0] << " " << bbsimd33[1] << " " << bbsimd33[2] << " " << bbsimd33[3] << " " << bbsimd33[4] << " " << bbsimd33[5] << "      " << d3.count() * 1000 / rep << "ms" << std::endl;
	std::cout << "simd2 speed up " << d1.count() / d3.count() << "X" << std::endl;
}

void smmain()
{
	const int n = 1000000;
	std::cout << std::endl;
	std::cout << "计算" << addcommas(n) << " 个数字的平均值" << std::endl;
	std::random_device rnd_device;
	std::mt19937 mersenne_engine{ rnd_device() };

	std::uniform_real_distribution<float> distribution(0, 1);



	std::vector<float> data(n);
	std::vector<float> result0(n);
	std::vector<float> result1(n);
	std::vector<float> result2(n);

	std::generate(data.begin(), data.end(), [&]() { return distribution(mersenne_engine); });
	//std::iota(data.begin(), data.end(), 0);

	sm(data.data(), n);
	float result_scalar, result_simd;
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < rep; i++)
		result_scalar = sm(data.data(), n);
	auto md = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < rep; i++)
		result_simd = smsimd(data.data(), n);
	auto ed = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> d1 = (md - start);
	std::chrono::duration<double> d2 = (ed - md);

	std::cout << "scalar " << result_scalar / n << "      " << d1.count() * 1000 / rep << "ms" << std::endl;
	std::cout << "simd   " << result_simd / n << "      " << d2.count() * 1000 / rep << "ms" << std::endl;
	std::cout << "speed up " << d1.count() / d2.count() << "X" << std::endl;
}

void lpmain()
{

	const int n = 1000000;
	std::cout << std::endl;
	std::cout << "筛选出" << addcommas(n) << "个数字中小于threshold的元素并存入新地址" << std::endl;
	std::random_device rnd_device;
	std::mt19937 mersenne_engine{ rnd_device() };

	std::uniform_real_distribution<float> distribution(-1, 1);


	std::vector<float> data(n);
	std::vector<float> result0(n);
	std::vector<float> result1(n);
	std::vector<float> result2(n);

	std::generate(data.begin(), data.end(), [&]() { return distribution(mersenne_engine); });
	//std::iota(data.begin(), data.end(), 0);
	{
		float limit = 0.26f;
		std::cout << "with threshold: " << limit << std::endl;
		lp(data.data(), result0.data(), n);
		int num1, num2;
		auto start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < rep; i++)
			num1 = lp(data.data(), result1.data(), n, limit);;
		auto md = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < rep; i++)
			num2 = lpsimd(data.data(), result2.data(), n, limit);;
		auto ed = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> d1 = (md - start);
		std::chrono::duration<double> d2 = (ed - md);

		std::cout << "scalar " << num1 << "      " << d1.count() * 1000 / rep << "ms" << std::endl;
		std::cout << "simd   " << num2 << "      " << d2.count() * 1000 / rep << "ms" << std::endl;
		std::cout << "speed up " << d1.count() / d2.count() << "X" << std::endl;
	}

	{
		float limit = -0.333f;
		std::cout << "with threshold: " << limit << std::endl;
		lp(data.data(), result0.data(), n);
		int num1, num2;
		auto start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < rep; i++)
			num1 = lp(data.data(), result1.data(), n, limit);;
		auto md = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < rep; i++)
			num2 = lpsimd(data.data(), result2.data(), n, limit);;
		auto ed = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> d1 = (md - start);
		std::chrono::duration<double> d2 = (ed - md);

		std::cout << "scalar " << num1 << "      " << d1.count() * 1000 / rep << "ms" << std::endl;
		std::cout << "simd   " << num2 << "      " << d2.count() * 1000 / rep << "ms" << std::endl;
		std::cout << "speed up " << d1.count() / d2.count() << "X" << std::endl;
	}

}
void smImgScalar(const unsigned char data1[], const unsigned char data2[], unsigned char result[], int n)
{
	for (int i = 0; i < n; i++)
	{
		int temp = data1[i];
		temp += data2[i];
		if (temp > 255)
			temp = 255;
		result[i] = temp;
	}
}

void smImgSimd(const unsigned char data1[], const unsigned char data2[], unsigned char result[], int n)
{
	for (int i = 0; i < n; i += 32)
	{
		__m256i t1 = _mm256_loadu_si256((__m256i const*)(data1 + i));
		__m256i t2 = _mm256_loadu_si256((__m256i const*)(data2 + i));
		__m256i t3 = _mm256_adds_epu8(t1, t2);
		_mm256_storeu_si256((__m256i*)(result + i), t3);
	}
}

void addImgMain()
{
	const int w = 1920;
	const int h = 1080;
	std::cout << std::endl;
	std::cout << "Image Add " << addcommas(w) << " * " << addcommas(h) << " uchars" << std::endl;
	std::random_device rnd_device;
	std::mt19937 mersenne_engine{ rnd_device() };

	std::uniform_int_distribution<int> distribution(0, 255);

	std::vector<unsigned char> data1(w * h);
	std::vector<unsigned char> data2(w * h);
	std::vector<unsigned char> result0(w * h);
	std::vector<unsigned char> result1(w * h);
	std::vector<unsigned char> result2(w * h);
	std::generate(data1.begin(), data1.end(), [&]() { return (unsigned char)distribution(mersenne_engine); });
	std::generate(data2.begin(), data2.end(), [&]() { return (unsigned char)distribution(mersenne_engine); });

	smImgScalar(data1.data(), data2.data(), result0.data(), w * h);
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < rep; i++)
		smImgScalar(data1.data(), data2.data(), result1.data(), w * h);
	auto md = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < rep; i++)
		smImgSimd(data1.data(), data2.data(), result2.data(), w * h);
	auto ed = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> d1 = (md - start);
	std::chrono::duration<double> d2 = (ed - md);

	std::cout << "scalar  " << d1.count() * 1000 / rep << "ms" << std::endl;
	std::cout << "simd    " << d2.count() * 1000 / rep << "ms" << std::endl;
	std::cout << "speed up " << d1.count() / d2.count() << "X" << std::endl;
}




int main()
{
	std::cout << "repeat " << rep << std::endl;

	smmain();
	lpmain();
	bbmain();
	
	//smmain();
	//lpmain();
	//bbmain();
	//addImgMain();
	std::cout << std::endl << "done " << std::endl;
	//nvmain();
	int c;
	std::cin >> c;
	//long long n = 10000000;
	//long long testc = 0;
	//long long testr = 0;
	//std::random_device rnd_device;
	//std::mt19937 mt{ rnd_device() };
	//std::uniform_int_distribution<int> d2(1, 2);
	//std::uniform_int_distribution<int> d1(0, 1);


	//for (int i = 0; i < n; i++)
	//{
	//	int r = d2(mt);
	//	if (r == 0)
	//		continue;
	//	if (r == 1)
	//	{
	//		int r = d1(mt);
	//		if (r == 0)
	//			testc++;
	//	}
	//	else
	//	{
	//		testr++;
	//		testc++;
	//	}
	//}

	//std::cout << 1.0f * testr / testc << std::endl;
	//int c;
	//std::cin >> c;

}


#include "gmock/gmock.h"

#include <chrono>



using namespace testing;
using namespace std::chrono;



TEST(Chrono_Test, time)
{
	time_point<steady_clock> t0 = steady_clock::now();
	int ni = 10000;
	int nj = 10000;
	for (int i = 0; i < ni; i++)
	{
		int sum = 0;
		for (int j = 0; j < nj; j++)
		{
			sum++;
		}
	}
	auto t1 = steady_clock::now();
	duration<double> df = (t1 - t0);

	float time = df.count();
	std::cout << time << std::endl;
	std::cout << ni * nj * 4 / time / 1e9 <<" Gbytes/s "<< std::endl;

}
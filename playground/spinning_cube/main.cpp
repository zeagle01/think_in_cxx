
#include <iostream>
#include <vector> 
#include <thread>
#include <algorithm>  
#include <array>  


template< int N, int M>
struct mat
{
	std::array<float, N* M> data;

	const float& operator()(int i, int j) const
	{
		return data[i * M + j];
	}

	float& operator()(int i, int j)
	{
		return data[i * M + j];
	}
};

template< int N >
struct mat<N,1>
{
	std::array<float, N> data;

	const float& operator()(int i, int j) const
	{
		return data[i * 1 + j];
	}

	float& operator()(int i, int j)
	{
		return data[i * 1 + j];
	}

	const float& operator()(int i) const
	{
		return data[i ];
	}

	float& operator()(int i)
	{
		return data[i];
	}
};

template< int N, int L,int M>
static mat<N, M> operator*(const mat<N, L> a, const mat<L, M> b)
{
	mat<N, M> ret;
	for (int ni = 0; ni < N; ni++)
	{
		for (int mi = 0; mi < M; mi++)
		{
			ret(ni,mi) = 0.f;
			for (int li = 0; li < L; li++)
			{
				ret(ni, mi) += a(ni, li) * b(li, mi);
			}
		}
	}
	return ret;
}

using vec3 = mat<3, 1>;
using mat3 = mat<3, 3>;



class Spinning_Cube
{
public:
	void run()
	{
		send_cursor_home_position();

		while (true)
		{
			buffer.assign(width * height, ' ');
			zbuffer.assign(width * height, 0.f);
			for (float cube_x = -cube_width; cube_x < cube_width; cube_x += cube_unit)
			{
				for (float cube_y = -cube_width; cube_y < cube_width; cube_y += cube_unit)
				{
					calculate_point(cube_x, cube_y, -cube_width, '@'); //back side of the cube
					calculate_point(cube_x, cube_y, cube_width, '#'); //
					calculate_point(cube_width, cube_x, cube_y, '$'); //
					calculate_point(-cube_width, cube_x, cube_y, '~'); //
					calculate_point(cube_x, -cube_width, cube_y, '+'); //
					calculate_point(cube_x, cube_width, cube_y, ';'); //
				}
			}

			print_buffer();

			A += 0.05;
			B += 0.05;
			C += 0.01;

			using namespace std::chrono_literals;
			std::this_thread::sleep_for(10ms);
		}

	}

private:
	void send_cursor_home_position()
	{
		//printf("\x1b[2J");
		printf("\x1b[H");
	}

	void print_buffer()
	{
		//for (int i = 0; i < width * height; i++)
		//{
		//	putchar(i % width ? buffer[i] : '\n');
		//}

		for (int i = 0; i < width * height; i++)
		{
			buffer[i] = i % width ? buffer[i] : '\n';
		}
		fwrite(buffer.data(), 1, width * height, stdout);
	}

	void calculate_point(float cubeX, float cubeY, float cubeZ, char ch)
	{

		using namespace std;
		mat3 rx
		{
			1.f,	0.0,		0.0,
			0.f,	cos(A),		sin(A),
			0.f,	-sin(A),	cos(A)
		};

		mat3 ry
		{
			cos(B),		0.0,	sin(B),
			0.f,		1.f,	0.f,
			-sin(B),	0.f,	cos(B)
		};

		mat3 rz
		{
			cos(C),		sin(C),		0.f,
			-sin(C),	cos(C),		0.f,
			0.f,		0.f,		1.f
		};

		vec3 pos{ cubeX,cubeY,cubeZ };
		vec3 pos_screen = rz * ry * rx * pos;

		//view transformatjion
		float x = pos_screen(0);
		float y = pos_screen(1);
		float z = pos_screen(2) + distance_from_camara;

		//projection
		float ooz = 1 / z;
		int x_screen = (int)(width / 2 + K1 * ooz * x*2);
		int y_screen = (int)(height / 2 + K1 * ooz * y);

		int idx = x_screen + y_screen * width;
		if (idx >= 0 && idx < width * height) //on screen range
		{
			if (ooz > zbuffer[idx])// depth test only keep the nearest pixel
			{
				zbuffer[idx] = ooz;
				buffer[idx] = ch;
			}
		}

	}


private:
	std::vector<char > buffer;
	std::vector<float > zbuffer;
	int width = 60;
	int height = 30;
	int distance_from_camara = 100;

	float cube_width = 20.f;
	float cube_unit = 0.6f;

	float A = 0.f;
	float B = 0.f;
	float C = 0.f;
	float K1 = 30.f;
};

int main()
{
	Spinning_Cube sc;
	sc.run();
	return 0;
}



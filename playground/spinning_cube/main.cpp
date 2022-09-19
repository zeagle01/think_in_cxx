
#include <iostream>
#include <vector> 
#include <thread>
#include <algorithm>  
#include <array>  


#define real float

template< int N, int M>
struct mat
{
	std::array<real, N* M> data;

	const real& operator()(int i, int j) const
	{
		return data[i * M + j];
	}

	real& operator()(int i, int j)
	{
		return data[i * M + j];
	}
};

template< int N >
struct mat<N,1>
{
	std::array<real, N> data;

	const real& operator()(int i, int j) const
	{
		return data[i * 1 + j];
	}

	real& operator()(int i, int j)
	{
		return data[i * 1 + j];
	}

	const real& operator()(int i) const
	{
		return data[i ];
	}

	real& operator()(int i)
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

template<int N,int M>
static mat<N, M> operator-(const mat<N, M>& a, const mat<N, M>& b)
{
	mat<N, M> ret;
	for (int i = 0; i < N * M; i++)
	{
		ret.data[i] = a.data[i] - b.data[i];
	}
	return ret;
}

template<int N,int M>
static mat<N, M> operator+(const mat<N, M>& a, const mat<N, M>& b)
{
	mat<N, M> ret;
	for (int i = 0; i < N * M; i++)
	{
		ret.data[i] = a.data[i] + b.data[i];
	}
	return ret;
}

template<int N,int M>
static real length(const mat<N, M>& a)
{
	real l = 0;
	for (int i = 0; i < N * M; i++)
	{
		l += a.data[i] * a.data[i];
	}
	return std::sqrt(l);
}

template<int N>
static mat<N,1> normalized(const mat<N, 1>& a)
{
	real l = length(a);
	mat<N, 1> ret;
	for (int i = 0; i < N; i++)
	{
		ret.data[i] = a.data[i] / l;
	}
	return ret;
}

static mat<3, 1> cross(const mat<3, 1>& a, const mat<3, 1>& b)
{
	mat<3, 1> ret;
	ret(0) = a(1) * b(2) - a(2) * b(1);
	ret(1) = b(0) * a(2) - b(2) * a(0);
	ret(2) = a(0) * a(1) - a(1) * b(0);
	return ret;
}



template< int N >
static mat<N, N> get_identity()
{
	mat<N, N> ret;
	for (int i = 0; i < N * N; i++)
	{
		ret.data[i] = 0.f;
	}
	for (int i = 0; i < N; i++)
	{
		ret(i, i) = 1.f;
	}
	return ret;
}

using vec3 = mat<3, 1>;
using mat3 = mat<3, 3>;
using vec4 = mat<4, 1>;
using mat4 = mat<4, 4>;



class Spinning_Cube
{
public:
	void run()
	{
		send_cursor_home_position();

		while (true)
		{
			buffer.assign(viewport_width * viewport_height, ' ');
			zbuffer.assign(viewport_width * viewport_height, 0.f);

			for (real cube_x = -cube_width; cube_x < cube_width; cube_x += cube_unit)
			{
				for (real cube_y = -cube_width; cube_y < cube_width; cube_y += cube_unit)
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
			std::this_thread::sleep_for(1ms);
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
		for (int i = 0; i < viewport_width * viewport_height; i++)
		{
			buffer[i] = i % viewport_width ? buffer[i] : '\n';
		}
		fwrite(buffer.data(), 1, viewport_width * viewport_height, stdout);
	}

	mat4 get_rotation_x()
	{
		mat4 ret = get_identity<4>();
		float c = std::cos(A);
		float s = std::sin(A);
		ret(1, 1) = c;
		ret(1, 2) = s;
		ret(2, 1) = -s;
		ret(2, 2) = c;
		return ret;
	}

	mat4 get_rotation_y()
	{
		mat4 ret = get_identity<4>();
		float c = std::cos(B);
		float s = std::sin(B);
		ret(0, 0) = c;
		ret(0, 2) = s;
		ret(2, 0) = -s;
		ret(2, 2) = c;
		return ret;
	}

	mat4 get_rotation_z()
	{
		mat4 ret = get_identity<4>();
		float c = std::cos(B);
		float s = std::sin(B);
		ret(0, 0) = c;
		ret(0, 1) = s;
		ret(1, 0) = -s;
		ret(1, 1) = c;
		return ret;
	}

	mat4 get_view_matrix()
	{
		mat4 ret = get_identity<4>();
		vec3 z = normalized(camara_position - look_at_posiiton);
		vec3 x = normalized(cross(up, z));
		vec3 y = normalized(cross(z, x));
		ret(0, 0) = x(0);
		ret(1, 0) = x(1);
		ret(2, 0) = x(2);

		ret(0, 1) = y(0);
		ret(1, 1) = y(1);
		ret(2, 1) = y(2);

		ret(0, 2) = z(0);
		ret(1, 2) = z(1);
		ret(2, 2) = z(2);

		float perspective_factor = 1.f;

		ret(2, 0) *= perspective_factor;
		ret(2, 1) *= perspective_factor;
		ret(2, 2) *= perspective_factor;

		mat4 inverse_translate = get_identity<4>();
		inverse_translate(0, 3) = -camara_position(0);
		inverse_translate(1, 3) = -camara_position(1);
		inverse_translate(2, 3) = -camara_position(2);


		ret = ret * inverse_translate;
		return ret ;
	}

	mat4 get_perspective_matrix()
	{
		mat4 ret = get_identity<4>();
		ret(0, 0) = view_near;
		ret(1, 1) = view_near;
		ret(2, 2) = view_near + view_far;
		ret(3, 3) = 0.f;

		ret(2, 3) = -view_near * view_far;
		ret(3, 2) = 1.f;
		return ret;
	}


	mat4 compute_view_volumn_transform_matrix(const vec3& x0_from, const vec3& x1_from, const vec3& x0_to, const vec3& x1_to)
	{
		mat4 inverse_t0 = get_identity<4>();
		inverse_t0(0, 3) = -x0_from(0);
		inverse_t0(1, 3) = -x0_from(1);
		inverse_t0(2, 3) = -x0_from(2);

		mat4 scale = get_identity<4>();
		scale(0, 0) = (x1_to(0) - x0_to(0)) / (x1_from(0) - x0_from(0));
		scale(1, 1) = (x1_to(1) - x0_to(1)) / (x1_from(1) - x0_from(1));
		scale(2, 2) = (x1_to(2) - x0_to(2)) / (x1_from(2) - x0_from(2));

		mat4 t1 = get_identity<4>();

		t1(0, 3) = x0_to(0);
		t1(1, 3) = x0_to(1);
		t1(2, 3) = x0_to(2);

		return t1 * scale * inverse_t0;

	}

	float degree_2_radian(float degree)
	{
		const float pi = 3.1415926;
		return  degree * pi / 180.f;
	}

	mat4 get_viewport_matrix()
	{
		vec3 x0_from{ -1,-1,-1 };
		vec3 x1_from{ 1,1,1 };

		vec3 x0_to{ 0,0, view_near };
		vec3 x1_to{ viewport_width,viewport_height,view_far };

		return compute_view_volumn_transform_matrix(x0_from, x1_from, x0_to, x1_to);

	}

	mat4 get_view_volumn_matrix()
	{
		float half_space_height = std::abs(view_near) * std::tan(degree_2_radian(fov / 2.f));
		float half_space_width = half_space_height * aspect;

		vec3 x0_from{ -half_space_width,-half_space_height,view_near };
		vec3 x1_from{ half_space_width,half_space_height,view_far };

		vec3 x0_to{ -1,-1,-1 };
		vec3 x1_to{ 1,1,1 };

		return compute_view_volumn_transform_matrix(x0_from, x1_from, x0_to, x1_to);
	}

	void calculate_point(real cubeX, real cubeY, real cubeZ, char ch)
	{

		auto rx = get_rotation_x();
		auto ry = get_rotation_y();
		auto rz = get_rotation_z();

		vec4 pos{ cubeX,cubeY,cubeZ,1.f };

		auto view_matrix = get_view_matrix();

		auto perspective_matrix = get_perspective_matrix();
		auto view_volumn_matrix = get_view_volumn_matrix();
		auto viewport_matrix = get_viewport_matrix();

		vec4 p0 = view_matrix * (rz * ry * rx) * pos;
		vec4 p1 = view_volumn_matrix * perspective_matrix * p0;
		vec4 p2 = viewport_matrix * p1;
		vec4 pos_screen =  p2;

		float x = pos_screen(0) / pos_screen(3);
		float y = pos_screen(1) / pos_screen(3);
		float z = pos_screen(2) / pos_screen(3);
		float ooz = 1.f / z;


		int x_screen = (int)(x);
		int y_screen = (int)(y);

		int idx = x_screen + y_screen * viewport_width;

		if (idx >= 0 && idx < viewport_width * viewport_height) //on screen range
		{
			if (std::abs(ooz) > std::abs(zbuffer[idx]))// depth test only keep the nearest pixel
			{

				zbuffer[idx] = std::abs(ooz);
				buffer[idx] = ch;
			}
		}

	}


private:
	std::vector<char > buffer;
	std::vector<real > zbuffer;
	int viewport_width = 60;
	int viewport_height = 30;

	vec3 camara_position{ 0.f,0.f,80.f };
	vec3 up{ 0.f,1.f,0.f };
	vec3 look_at_posiiton{ 0.f,0.f,0.f };

	real view_near		= -0.1f;
	real view_far		= -1e3f;

	real fov = 50.f;
	real pixel_aspect = 0.5f;// not a square pixel
	real aspect = pixel_aspect * viewport_width / viewport_height;


	real cube_width = 20.f;
	real cube_unit = 0.6f;

	real A = 0.f;
	real B = 0.f;
	real C = 0.f;

};

int main()
{
	Spinning_Cube sc;
	sc.run();
	return 0;
}



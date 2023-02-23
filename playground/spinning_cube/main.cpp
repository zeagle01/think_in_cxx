
#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>


template<int N,int M>
struct mat;

template<int N,int M>
struct mat_view
{
	using fptr = float*;
	fptr dataRef[N * M];

	float& operator()(int i, int j) { return *dataRef[i * M + j]; }
	const float& operator()(int i, int j) const { return *dataRef[i * M + j]; }

	void register_ref(int i, int j, float* v_ptr) { dataRef[i * M + j] = v_ptr; }

	operator mat<N, M>() 
	{
		mat<N, M> ret;
		for (int i = 0; i < N * M; i++)
		{
			ret.data[i] = *dataRef[i];
		}
		return ret;
	}

	mat_view& operator=(const mat<N, M>& a);
};


template<int N,int M>
struct mat
{
	float data[N * M];

	float& operator()(int i, int j) { return data[i * M + j]; }
	const float& operator()(int i, int j) const { return data[i * M + j]; }

	template<int bN,int bM>
	mat_view<bN, bM> block(int i0, int j0)
	{
		mat_view<bN, bM> view;
		for (int i = 0; i < bN; i++)
		{
			for (int j = 0; j < bM; j++)
			{
				view.register_ref(i, j, &(operator()(i0 + i, j0 + j)));
			}
		}
		return view;
	}
};

template<int N, int M>
mat_view<N,M>& mat_view<N, M>::operator=(const mat<N, M>& a)
{
	for (int i = 0; i < N * M; i++)
	{
		*(dataRef[i]) = a.data[i];
	}
	return *this;
}


template<int N>
struct mat<N, 1>
{

	float data[N ];

	float& operator()(int i, int j) { return data[i * 1 + j]; }
	const float& operator()(int i, int j) const { return data[i * 1 + j]; }

	float& operator()(int i) { return data[i]; }
	const float& operator()(int i) const { return data[i]; }

};



template< int N,int L,int M>
mat<N, M> operator*(const mat<N, L>& a, const mat<L, M>& b)
{
	mat<N, M> ret;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			ret(i, j) = 0;
			for (int l = 0; l < L; l++)
			{
				ret(i, j) += a(i, l) * b(l, j);
			}
		}
	}
	return ret;
}

template< int N,int M>
mat<N, M> operator+(const mat<N, M>& a, const mat<N, M>& b)
{
	mat<N, M> ret;
	for (int i = 0; i < N * M; i++)
	{
		ret.data[i] = a.data[i] + b.data[i];
	}
	return ret;
}

template< int N,int M>
mat<N, M> operator-(const mat<N, M>& a, const mat<N, M>& b)
{
	mat<N, M> ret;
	for (int i = 0; i < N * M; i++)
	{
		ret.data[i] = a.data[i] - b.data[i];
	}
	return ret;
}

template< int N,int M>
mat<N, M> operator-(const mat<N, M>& a)
{
	mat<N, M> ret;

	for (int i = 0; i < N * M; i++)
	{
		ret.data[i] = -a.data[i];
	}

	return ret;
}

mat<3, 1> cross(const mat<3, 1>& a, const mat<3, 1>& b)
{
	mat<3, 1> ret;
	ret(0) = a(1) * b(2) - a(2) * b(1);
	ret(1) = -a(0) * b(2) + a(2) * b(0);
	ret(2) = a(0) * b(1) - a(1) * b(0);
	return ret;
}

template<int N,int M> 
float dot(const mat<N, M>& a, const mat<N, M>& b)
{
	float ret = 0.f;
	for (int i = 0; i < N * M; i++)
	{
		ret += a.data[i] * b.data[i];
	}
	return ret;
}

template<int N,int M>
float length(const mat<N, M>& a)
{
	return std::sqrt(dot(a, a));
}

template<int N>
mat<N, 1> normalize(const mat<N, 1>& a)
{
	mat<N, 1> ret;

	float l = length(a);

	for (int i = 0; i < N; i++)
	{
		ret(i) = a(i) / l;
	}

	return ret;
}



template<int N>
mat<N, N> get_identity()
{
	mat<N, N> ret;

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (i == j)
			{
				ret(i, j) = 1.f;
			}
			else
			{
				ret(i, j) = 0.f;
			}
		}
	}

	return ret;
}


using mat4 = mat<4, 4>;
using mat3 = mat<3, 3>;
using vec4 = mat<4, 1>;
using vec3 = mat<3, 1>;




class Screen_Buffer
{
public:
	void set_size(int width, int height)
	{
		this->width = width;
		this->height = height;
		data.resize((width + 1) * height);

		for (int j = 0; j < height; j++)
		{
			operator()(j, width) = '\n';
		}
	}

	void clear(char c)
	{
		for (int i = 0; i< width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				operator()(j, i) = c;
			}
		}
	}

	void flush_out()
	{
		fwrite(data.data(), 1, data.size(), stdout);
	}

	char& operator() (int i, int j) { return data[i * (width + 1) + j]; }
	const char& operator() (int i, int j) const { return data[i * (width + 1) + j]; }

private:
	std::vector<char> data;
	int width;
	int height;
};



class Spinning_Cube
{
public:
	void run()
	{

		move_cursor_home();

		buffer.set_size(width, height);
		depth_buffer.resize(width * height);

		while (true)
		{
			buffer.clear(' ');
			depth_buffer.assign(width * height, 0.f);

			draw_cube();

			buffer.flush_out();

			angle_x += 0.02f;
			angle_y += 0.02f;
			angle_z += 0.005f;

			using namespace std::chrono_literals;
			std::this_thread::sleep_for(10ms);
		}
	}

private:

	mat4 get_rotation_x()
	{
		float c = std::cos(angle_x);
		float s = std::sin(angle_x);
		mat4 rot =
		{
			1.f,0.f,0.f,0.f,
			0.f,c,s,0.f,
			0.f,-s,c,0.f,
			0.f,0.f,0.f,1.f
		};
		return rot;
	}

	mat4 get_rotation_y()
	{
		float c = std::cos(angle_y);
		float s = std::sin(angle_y);
		mat4 rot =
		{
			c,0.f,s,0.f,
			0.f,1.f,0.f,0.f,
			-s,0.f,c,0.f,
			0.f,0.f,0.f,1.f
		};
		return rot;
	}

	mat4 get_rotation_z()
	{
		float c = std::cos(angle_z);
		float s = std::sin(angle_z);
		mat4 rot =
		{
			c,s,0.f,0.f,
			-s,c,0.f,0.f,
			0.f,0.f,1.f,0.f,
			0.f,0.f,0.f,1.f
		};
		return rot;
	}

	mat4 get_view_matrix()
	{
		mat4 rotation = get_identity<4>();


		auto z = normalize(camara_pos - look_at);
		auto x = normalize(cross(up, z));
		auto y = normalize(cross(z, x));

		rotation.block<3, 1>(0, 0) = x;
		rotation.block<3, 1>(0, 1) = y;
		rotation.block<3, 1>(0, 2) = z;

		mat4 inverse_translate = get_identity<4>();
		inverse_translate.block<3, 1>(0, 3) = -camara_pos;

		return  rotation * inverse_translate;
	}

	float degree_2_radian(float d)
	{
		const float pi = 3.1415926f;
		return d * pi / 180.f;
	}

	mat4 get_viewport_matrix()
	{
		mat4 ret = get_identity<4>();

		float t = std::tan(degree_2_radian(fov) / 2.f);
		float half_space_height = std::abs(view_near) * t;
		float half_space_width = half_space_height * aspect;

		vec3 x_low_from{ -half_space_width,-half_space_height,view_near };
		vec3 x_high_from{ half_space_width,half_space_height,view_far };

		vec3 x_low_to{ 0,height,view_near };
		vec3 x_high_to{ width,0,view_far };

		mat4 move_back_to_origin = get_identity<4>();
		move_back_to_origin.block<3, 1>(0, 3) = -x_low_from;

		mat4 scale = get_identity<4>();
		for (int i = 0; i < 3; i++)
		{
			scale(i, i) = (x_high_to(i) - x_low_to(i)) / (x_high_from(i) - x_low_from(i));
		}

		mat4 move_to_to = get_identity<4>();
		move_to_to.block<3, 1>(0, 3) = x_low_to;

		return move_to_to * scale * move_back_to_origin;
	}

	mat4 get_perspective_matrix()
	{
		mat4 ret = get_identity<4>();

		ret(0, 0) = view_near;
		ret(1, 1) = view_near;
		ret(2, 2) = view_near + view_far;
		ret(3, 3) = 0.f;
		ret(3, 2) = 1.f;
		ret(2, 3) = -view_near * view_far;

		return ret;
	}


	void draw_pixel(const vec3& x, const vec3& normal, char c)
	{
		vec4 pos{ x(0),x(1),x(2),1.0f };

		auto rot_x = get_rotation_x();
		auto rot_y = get_rotation_y();
		auto rot_z = get_rotation_z();
		pos = rot_z * rot_y * rot_x * pos;

		auto view_matrix = get_view_matrix();
		pos = view_matrix * pos;
		auto view_pos = pos;

		auto perspective_matrix = get_perspective_matrix();
		pos = perspective_matrix * pos;

		auto viewport_matrix = get_viewport_matrix();
		pos = viewport_matrix * pos;

		vec3 sx{ pos(0) / pos(3), pos(1) / pos(3), pos(2) / pos(3) };

		int xi = int(sx(0));
		int yi = int(sx(1));


		if (xi >= 0 && xi < width && yi >= 0 && yi < height)
		{
			float depth = 1.f / std::abs(sx(2));
			int idx = yi * width + xi;
			if (depth > depth_buffer[idx])
			{
				auto T = view_matrix * rot_z * rot_y * rot_x;

				auto n_homo = T * vec4{ normal(0), normal(1), normal(2), 0.f };
				auto p_homo = T * vec4{ x(0),x(1),x(2),1.0f };
				auto l_homo = view_matrix * vec4{ light_position(0), light_position(1), light_position(2), 1.f };

				auto l = vec3{ l_homo(0), l_homo(1), l_homo(2) };
				auto p = vec3{ p_homo(0), p_homo(1), p_homo(2) };
				auto n = vec3{ n_homo(0), n_homo(1), n_homo(2) };
				auto e = normalize(-p);

				auto l_dir = normalize(l - p);
				float diffuse = std::max(dot(n, l_dir), 0.f);

				auto h = normalize((l_dir + e));

				float specular = std::max(std::pow(dot(h, n), 3e2f), 0.f);

				float density = std::clamp(0.5f * diffuse + 0.5f * specular, 0.00f, 0.99f);

				depth_buffer[idx] = depth;

				auto density_char = get_density_char(density);
				buffer(yi, xi) = density_char;
			}
		}

	}

	void draw_cube()
	{
		for (float i = -cube_width / 2; i < cube_width / 2; i += cube_unit)
		{
			for (float j = -cube_width / 2; j < cube_width / 2; j += cube_unit)
			{
				draw_pixel({ i, j, cube_width / 2 }, { 0, 0, 1 },'F');
				draw_pixel({ i, j, -cube_width / 2 }, { 0, 0, -1 },'B');
				draw_pixel({ i, -cube_width / 2, j }, { 0,-1,0 },'D');
				draw_pixel({ i, cube_width / 2, j }, { 0,1,0 },'U');
				draw_pixel({ -cube_width / 2, i, j }, { -1,0,0 },'L');
				draw_pixel({ cube_width / 2, i, j }, { 1,0,0 }, 'R');
			}
		}
	}

	void move_cursor_home()
	{
		printf("\x1b[H");
	}

	static char get_density_char(float d)
	{
		static const char density[] = { '@','#','W','$','9','8','6','5','3','2','0','4','7','?','a','b','c','1','!',';',':','+','=',',','.','_' };

		int size = sizeof(density);
		int p = (int)size * d;
		return density[p];
	}

	int width = 90;
	int height = 45;
	Screen_Buffer buffer;
	std::vector<float> depth_buffer;

	float cube_width = 20;
	float cube_unit = 0.3;

	vec3 light_position{ 5,1,20 };

	vec3 up{ 0, 1, 0 };
	vec3 camara_pos{ 0, 0, 100 };
	vec3 look_at{ 0, 0, 0 };

	float view_near = -0.01f;
	float view_far = -1e3f;
	float fov = 20.f;

	float pixel_aspect = 0.5f;
	float aspect = pixel_aspect * width / height;

	float angle_x = 0.f;
	float angle_y = 0.f;
	float angle_z = 0.f;

};


int main()
{
	Spinning_Cube cube;

	cube.run();

	return 0;
}







#include "gmock/gmock.h"


using namespace testing;

namespace vector_test
{


	template<typename T>
	class Vector
	{
	public:

		~Vector() 
		{ 
			delete[] data; 
		}
		Vector() = default;
		Vector(std::initializer_list<T> l)
		{

			m_size = l.size();
			m_capacity = m_size;

			int i = 0;
			data = new T[m_size];
			for (const auto& it : l)
			{
				data[i] = it;
				i++;
			}
		}

		Vector(const Vector& other)
		{
			m_size = other.m_size;
			m_capacity = other.m_capacity;
			data = new T[m_size];
			for (int i = 0; i < m_size; i++)
			{
				data[i] = other.data[i];
			}
		}

		Vector& operator=(const Vector& other)
		{
			m_size = other.m_size;
			m_capacity = other.m_capacity;

			delete[] data;

			data = new T[m_size];
			for (int i = 0; i < m_size; i++)
			{
				data[i] = other.data[i];
			}
			return *this;

		}


		void push_back(const T& v)
		{
			try_resize();
			data[m_size] = v;
			m_size++;
		}


		const T& operator[](int i) const
		{
			return data[i];
		}

		T& operator[](int i) 
		{
			return data[i];
		}

		int size()
		{
			return m_size;
		}

		int capacity()
		{
			return m_capacity;
		}
	private:
		void try_resize()
		{

			if (m_size >= m_capacity)
			{
				if (m_size == 0)
				{
					m_capacity++;
					data = new T[m_capacity];
				}
				else
				{
					m_capacity *= 2;

					T* old = data;
					data = new T[m_capacity];
					for (int i = 0; i < m_size; i++)
					{
						data[i] = old[i];
					}
					delete[] old;
				}

			}

		}

	private:
		T* data = nullptr;
		int m_size = 0;
		int m_capacity = 0;

	public:
		template<typename T>
		friend static bool operator==(const Vector<T>& l, const Vector<T>& r)
		{
			if (l.m_size != r.m_size)
			{
				return false;
			}
			else
			{
				for (int i = 0; i < l.m_size; i++)
				{
					if (l[i] != r[i])
					{
						return false;
					}
				}
			}
			return true;
		}
	};



	TEST(Vector_Test, default_constructed_size_is_0)
	{
		Vector<int> v;
		EXPECT_THAT(v.size(), Eq(0));
	}

	TEST(Vector_Test, push_one_element_size_is_1)
	{

		Vector<int> v;
		v.push_back(0);
		EXPECT_THAT(v.size(), Eq(1));
	}

	TEST(Vector_Test, get_value_from_pushed)
	{

		Vector<int> v;
		int e = 1;
		v.push_back(e);
		EXPECT_THAT(v[0], Eq(e));
	}

	TEST(Vector_Test, capacity_grows_when_pushed)
	{

		Vector<int> v;
		int e = 1;
		v.push_back(e);
		EXPECT_THAT(v.capacity(), Gt(0));
	}

	TEST(Vector_Test, initilizeer_list_to_construct)
	{
		Vector<int> v{ 0 };
		EXPECT_THAT(v[0], Eq(0));
	}

	TEST(Vector_Test, initilizeer_list_to_construct_with_size_2)
	{
		Vector<int> v{ 0 ,1};
		EXPECT_THAT(v[0], Eq(0));
		EXPECT_THAT(v[1], Eq(1));
	}

	TEST(Vector_Test, vector_equal_test)
	{
		Vector<int> v0{ 0 };
		Vector<int> v1{ 0 };
		EXPECT_THAT(v0, Eq(v1));
	}


	TEST(Vector_Test, vector_with_diff_value_not_equal)
	{
		Vector<int> v0{ 0 };
		Vector<int> v1{ 1 };
		EXPECT_THAT(v0, Ne(v1));
	}

	TEST(Vector_Test, vector_with_diff_size_not_equal)
	{
		Vector<int> v0{ 0 };
		Vector<int> v1{ 0,0 };
		EXPECT_THAT(v0, Ne(v1));
	}

	TEST(Vector_Test, push_two_element_cause_relocation_and_copy)
	{
		Vector<int> v;
		int e = 1;
		v.push_back(e);
		v.push_back(e);
		Vector<int> exp{ e,e };
		EXPECT_THAT(v, Eq(exp));
	}

	TEST(Vector_Test, copy_a_vector)
	{
		Vector<int> v0{ 1 };
		Vector<int> v1 = v0;
		EXPECT_THAT(v1, Eq(v0));
	}

	TEST(Vector_Test, copy_a_vector_within_block_to_check_dangling_pointer)
	{
		Vector<int> v;

		{
			Vector<int> v1{ 1 };
			v = v1;
		}
		Vector<int> exp{ 1 };
		EXPECT_THAT(v, Eq(exp));
	}

	TEST(Vector_Test, component_can_be_modify)
	{
		Vector<int> v{ 1 };
		v[0] = 2;

		EXPECT_THAT(v[0], Eq(2));
	}


	TEST(Vector_Test, copy_from_same_source_wont_change_each_other)
	{
		Vector<int> source{ 1 };
		Vector<int> v0 = source;
		Vector<int> v1 = source;
		v1[0] = 2;

		EXPECT_THAT(v0[0], Ne(2));
	}


}
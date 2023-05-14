
#include "gmock/gmock.h"

using namespace testing;

namespace share_ptr_test 
{

	template<typename T>
	class Shared_ptr;

	template<typename T>
	static Shared_ptr<T> make_shared(const T& v);

	template<typename T>
	class Shared_ptr
	{
		friend static Shared_ptr<T> make_shared<T>(const T& v); 
	public:

		Shared_ptr() = default;

		Shared_ptr<T>& operator=(Shared_ptr<T> other)
		{
			m_use_count = other.m_use_count;
			m_data = other.m_data;
			(*m_use_count)++;
			return *this;
		}

		T& operator*()
		{
			return *m_data;
		}

		operator bool() const
		{
			return m_data != nullptr;
		}

		int use_count()
		{
			return *m_use_count;

		}
	private:
		Shared_ptr(const T& v) 
		{ 
			m_data = new T(v); 
			m_use_count = new int(1);
		}

		T* m_data = nullptr;
		int* m_use_count = nullptr;

	};

	template<typename T>
	static Shared_ptr<T> make_shared(const T& v)
	{
		return Shared_ptr<T>(v);
	}


	TEST(Share_Ptr_Test, default_constructed_one_is_convertable_to_false)
	{
		Shared_ptr<int> p;

		EXPECT_FALSE(p);
	}

	TEST(Share_Ptr_Test, make_shared_is_convertable_to_true)
	{
		Shared_ptr<int> p = make_shared<int>(42);

		EXPECT_TRUE(p);
	}

	TEST(Share_Ptr_Test, make_shared_value_is_preserved)
	{
		Shared_ptr<int> p = make_shared<int>(42);

		EXPECT_THAT(*p, Eq(42));
	}

	TEST(Share_Ptr_Test, value_is_really_shared)
	{
		Shared_ptr<int> p = make_shared<int>(42);
		Shared_ptr<int> p1 = p;
		*p1 = 43;

		EXPECT_THAT(*p, Eq(43));
	}

	TEST(Share_Ptr_Test, use_count_is_1_when_created)
	{
		Shared_ptr<int> p = make_shared<int>(42);

		EXPECT_THAT(p.use_count(), Eq(1));
	}

	TEST(Share_Ptr_Test, assign_will_increase_use_count)
	{
		Shared_ptr<int> p0 = make_shared<int>(42);
		Shared_ptr<int> p1 = make_shared<int>(43);
		p0 = p1;

		EXPECT_THAT(p0.use_count(), Eq(2));
		EXPECT_THAT(p1.use_count(), Eq(2));
	}



}

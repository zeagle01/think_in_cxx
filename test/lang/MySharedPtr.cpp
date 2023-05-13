
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

		T& operator*()
		{
			return *data;
		}

		operator bool() const
		{
			return data != nullptr;
		}
	private:
		Shared_ptr(const T& v) { data = new T(v); }

		T* data = nullptr;

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
}

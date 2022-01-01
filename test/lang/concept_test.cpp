


#include "gmock/gmock.h"


#include <concepts>



using namespace testing;


namespace concept_test
{

	namespace hello_use
	{

		template<typename T>
		concept has_type = requires {typename T::type; };


		struct A
		{};


		struct B
		{
			using type = B;
		};


		TEST(Concept_Test, has_type)
		{
			//auto a = has_type<A<void>>;

			EXPECT_THAT(has_type<A>, Eq(false));
			EXPECT_THAT(has_type<B>, Eq(true));

		}

	}

	namespace Hendrik_cppcon2020
	{
		//requires clause
		template<typename FloatType>
		auto sum(const std::vector<FloatType>& values) requires std::floating_point<FloatType>
		{
			FloatType ret = 0.0;
			for (const auto value : values)
			{
				ret += value;
			}
			return ret;
		}

		


		TEST(Concept_Test,use_sum)
		{
			auto act = sum<float>({ 0,1,2 });

			EXPECT_THAT(act, Eq(3));

		}



		////////////
		template< int N> requires (N > 0)
			auto dimension()
		{
			return N;
		}

		TEST(Concept_Test,non_type_template_parameter)
		{
			auto act = dimension<2>();
			//act = dimension<-1>();// not satisfied the concept

			EXPECT_THAT(act, Eq(2));

		}



		//iterable with float
		template<typename T>
		concept  IterableWithFloat = std::floating_point<typename T::value_type> && std::forward_iterator<typename T::const_iterator>;

		template<IterableWithFloat T> 
		auto sum_1(T values)
		{
			typename T::value_type ret = 0.0;
			for (const auto value : values)
			{
				ret += value;
			}
			return ret;
		}

		TEST(Concept_Test,use_sum_with_concept)
		{
			auto act = sum_1(std::vector<float>{ 0,1,2 });

			EXPECT_THAT(act, Eq(3));

		}


		//define concept
		template<typename T>
		concept Addable = requires (T a, T b) { 
			{a + b} noexcept ->std::convertible_to<float>;
		};

		TEST(Concept_Test, addable_test)
		{
			auto act = Addable<int>;

			EXPECT_TRUE(Addable<int>);
		}

	}

	namespace string_template
	{

		template<typename Char,int N> 
		struct fix_string
		{
			Char data[N];
			constexpr fix_string(const Char(&str)[N]) { std::copy_n(str, N, data); }
		};

		template<fix_string s>
		struct print_fix_string
		{
			static void print_string(std::vector<std::string>& console)
			{
				console.push_back(std::string(s.data));
			}

		};

		template<fix_string s>
		constexpr void print_string(std::vector<std::string>& console) //this doesn't work, don't know why
		{
			console.push_back(std::string(s.data));
		}

		TEST(String_Template_Test, test_construct)
		{
			fix_string fs("hello");
		}

		TEST(String_Template_Test, test_print)
		{
			constexpr char str[] = "hello";
			std::vector<std::string> exp{ str };
			std::vector<std::string> act;
			print_fix_string<str>::print_string(act);
			//print_string<str>(act);//this doesn't work, don't know why
			EXPECT_THAT(exp, act);
		}

	}

}
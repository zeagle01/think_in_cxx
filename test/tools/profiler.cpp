
#include "gmock/gmock.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>





namespace profiler_test
{

//////////////////instrumentor

	struct Profile_Record
	{
		std::string name;
		long start;
		long end;
		uint32_t thread_id;
	};


	class Instrumentor
	{
	public:
		Instrumentor():m_profile_count(0)
		{
		}

		~Instrumentor()
		{
		}

		void begin_session(const std::string& name, const std::string& file_path = "result.json")
		{
			m_output_stream.open(file_path);
			write_header();

		}
		void write_profile(const Profile_Record& record)
		{
			if (m_profile_count++ > 0)
			{
				m_output_stream << ",";
			}

			std::string name = record.name;
			std::replace(name.begin(), name.end(), '"', '\'');
			m_output_stream << "{";
			m_output_stream << "\"cat\":\"function\",";
			m_output_stream << "\"dur\":"<<record.end-record.start<<",";
			m_output_stream << "\"name\":\""<<name<<"\",";
			m_output_stream << "\"ph\":\"X\",";
			m_output_stream << "\"pid\":0,";
			m_output_stream << "\"tid\":"<<record.thread_id<<",";
			m_output_stream << "\"ts\":"<<record.start;
			m_output_stream << "}";

			m_output_stream.flush();

		}
		void end_session()
		{
			write_footer();
			m_output_stream.close();
			m_profile_count = 0;

		}

		void write_footer()
		{
			m_output_stream << "]}";
			m_output_stream.flush();
		}

		void write_header()
		{
			m_output_stream << "{\"otherData\":{},\"traceEvents\":[";
			m_output_stream.flush();
		}

		static Instrumentor& get_singleton()
		{
			static std::unique_ptr<Instrumentor> singleton = std::make_unique<Instrumentor>();
			return *singleton.get();
		}

	private:
		std::ofstream m_output_stream;
		int m_profile_count;

	};

	class Timer
	{
	public: 
		Timer(const std::string& name) :m_name(name)
		{
			m_begin = std::chrono::steady_clock::now();
		}
		~Timer()
		{
			auto end = std::chrono::steady_clock::now();
			std::chrono::duration<double>d = (m_begin - end);
			long begin_t = std::chrono::time_point_cast<std::chrono::microseconds>(m_begin).time_since_epoch().count();
			long end_t = std::chrono::time_point_cast<std::chrono::microseconds>(end).time_since_epoch().count();

			uint32_t tid = std::hash<std::thread::id>{}(std::this_thread::get_id());
			Instrumentor::get_singleton().write_profile({ m_name,begin_t,end_t ,tid});
		}
	private:
		std::chrono::time_point<std::chrono::steady_clock> m_begin;
		std::string m_name;
	};



#define PROFILING 1
#if PROFILING
#define PROFILER_SCOPE(name) Timer timer_##__LINE__(name);
#define PROFIEL_FUNCITON() PROFILER_SCOPE(__FUNCSIG__)
#else
#define PROFIEL_FUNCITON() 
#endif










	int sum(int n)
	{

		PROFIEL_FUNCITON()

		int sum = 0;
		for (int i = 0; i < n; i++)
		{
			sum++;
		}
		return sum;
	}

	int sum_sqrt(int n)
	{
		PROFIEL_FUNCITON()

		int sum = 0;
		for (int i = 0; i < n; i++)
		{
			sum+=std::sqrt(i);
		}
		return sum;
	}



	void run_benchmark()
	{
		PROFIEL_FUNCITON();

		int n = 100000;
		std::thread a( [=]() {sum(n); });
		std::thread b( [=]() {sum_sqrt(n); });
		a.join();
		sum_sqrt(n);
		b.join();
	}





}

using namespace testing;
using namespace profiler_test;




TEST(ProfilerTest, sum_up_to_1000)
{
	Instrumentor::get_singleton().begin_session(__FUNCSIG__);
	run_benchmark();

//	auto a = sum(100);
//	auto a1 = sum_sqrt(100);
//	EXPECT_THAT(a, Ge(0));
//	EXPECT_THAT(a1, Ge(0));

	Instrumentor::get_singleton().end_session();


}
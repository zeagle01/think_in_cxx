
#include "gmock/gmock.h"
#include "tbb/tbb.h"

#include <vector>
#include <chrono>
#include <string>
#include <iostream>
#include <fstream>

#include <omp.h>

namespace parallel_log
{
	struct TimeLineRecord
	{
		std::string name;
		long start;
		long end;
		std::thread::id thread_id;
		//uint32_t thread_id;
	};


	class TimeLine
	{
	public:
		static TimeLine& GetSingleton()
		{
			static TimeLine tl;
			return tl;
		}

		void CollectRecord(const TimeLineRecord& record)
		{
			uint32_t tidInt = uint32_t(std::hash<std::thread::id>{}(record.thread_id));
			if (m_logRecords.count(tidInt))
			{
				m_logRecords[tidInt].push_back(record);
			}
			else
			{
				m_logRecords[tidInt] = std::vector<TimeLineRecord>(1, record);
			}
		}

		void set_out_put_file(std::string file_name)
		{
			m_file_name = file_name;
		}

		void flush()
		{
			BeginSession(m_file_name);
			for (const auto& it : m_logRecords)
			{
				for (int i = 0; i < it.second.size(); i++)
				{
					WriteRecord(it.second[i]);
				}
			}
			m_logRecords.clear();
		}

		void EndSession()
		{
			if (m_os.is_open())
			{
				WriteFooter();
				m_os.close();
				m_recordCount = 0;
			}
		}

	private:
		void BeginSession(const std::string& file_path)
		{
			if (!m_os.is_open())
			{
				m_os.open(file_path);
				WriteHeader();
			}
		}

		void WriteRecord(const TimeLineRecord& record)
		{
			if (m_recordCount++ > 0)
			{
				m_os << ",";
			}

			std::string name = record.name;
			std::replace(name.begin(), name.end(), '"', '\'');
			m_os << "{";
			m_os << "\"cat\":\"function\",";
			m_os << "\"dur\":" << record.end - record.start << ",";
			m_os << "\"name\":\"" << name << "\",";
			m_os << "\"ph\":\"X\",";
			m_os << "\"pid\":0,";
#ifdef WIN32
			m_os << "\"tid\":" << record.thread_id << ",";
#else
			uint32_t tidInt = uint32_t(std::hash<std::thread::id>{}(record.thread_id));
			m_os << "\"tid\":" << tidInt << ",";
#endif
			m_os << "\"ts\":" << record.start;
			m_os << "}";

			m_os.flush();

		}


		void WriteFooter()
		{
			m_os << "]}";
			m_os.flush();
		}

		void WriteHeader()
		{
			m_os << "{\"otherData\":{},\"traceEvents\":[";
			m_os.flush();
		}

		tbb::concurrent_unordered_map<uint32_t, std::vector<TimeLineRecord>> m_logRecords;
		std::ofstream m_os;
		int m_recordCount = 0;
		std::string m_file_name="test_time_line.json";

	};


	class Logger
	{
	public:
		Logger(const std::string& s) :m_name(s)
		{
			m_begin = std::chrono::steady_clock::now();
		}

		~Logger()
		{
			auto end = std::chrono::steady_clock::now();

			long begin_t = std::chrono::time_point_cast<std::chrono::microseconds>(m_begin).time_since_epoch().count();
			long end_t = std::chrono::time_point_cast<std::chrono::microseconds>(end).time_since_epoch().count();

			TimeLine::GetSingleton().CollectRecord({ m_name,begin_t,end_t ,std::this_thread::get_id() });
		}

		std::chrono::time_point<std::chrono::steady_clock> m_begin;
		std::string m_name;

	};




	void some_work(float& a, int n)
	{
		Logger log("some_work");
		for (int i = 0; i < n; i++)
		{
			a = a * 0.9f + 2.f;
		}
	}

	TEST(Parallel_Log, log_within_parallel_for)
	{
		int num = 1e6;
		int work_loads = 1e0;
		std::vector<float> data(num);
		auto& tl = TimeLine::GetSingleton();
		tl.set_out_put_file("tbb_parallel_for.json");

		tbb::task_scheduler_init init;

		tbb::parallel_for(
			0, num,
			[&data, work_loads](int i)
			{
				some_work(data[i], work_loads);
			}
		);

		TimeLine::GetSingleton().flush();
		TimeLine::GetSingleton().EndSession();
	}


	TEST(Parallel_Log, log_within_omp_for)
	{
		int num = 1e6;
		int work_loads = 1e0;
		std::vector<float> data(num);
		auto& tl = TimeLine::GetSingleton();
		tl.set_out_put_file("omp_for.json");


//#pragma omp parallel for num_threads(23)
#pragma omp parallel for 
		for (int i = 0; i < num; i++)
		{
			some_work(data[i], work_loads);
		}

		TimeLine::GetSingleton().flush();
		TimeLine::GetSingleton().EndSession();
	}

	TEST(Parallel_Log, log_within_serial_for)
	{
		int num = 1e6;
		int work_loads = 1e0;
		std::vector<float> data(num);
		auto& tl = TimeLine::GetSingleton();
		tl.set_out_put_file("serial_for.json");

		for (int i = 0; i < num; i++)
		{
			some_work(data[i], work_loads);
		}

		TimeLine::GetSingleton().flush();
		TimeLine::GetSingleton().EndSession();
	}

}

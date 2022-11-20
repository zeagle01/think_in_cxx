
#include "gmock/gmock.h"

#include <string>
#include <fstream>
#include <unordered_map>
#include <functional>
//#include<tuple>

using namespace testing;

namespace serializer
{

	class Serializer
	{
	public:
		inline void set_string(const std::string& s, int index = 0)
		{
			if (index >= m_contents.size())
			{
				m_contents.push_back(s);
			}
			else
			{
				m_contents[index] = s;
			}
		}

		inline const std::string get_string(int i = 0) const
		{
			if (i >= m_contents.size())
			{
				return "";
			}
			return m_contents[i];
		}

		inline void set_real(float s, int index = 0)
		{
			set_string(std::to_string(s), index);
		}

		inline const float get_real(int i = 0) const
		{
			return std::atof(get_string(i).c_str());
		}

		inline void set_int(int i, int index = 0) 
		{
			set_string(std::to_string(i), index);
		}
		inline int get_int(int i)
		{
			return std::atoi(get_string(i).c_str());
		}

		inline const int get_field_counts() const
		{
			return m_contents.size();
		}

		inline Serializer& operator[](const std::string& s)
		{
			if (!m_sub_nodes_index.count(s))
			{
				m_sub_nodes_index[s] = m_sub_nodes.size();
				m_sub_nodes.push_back({ s,Serializer() });
			}

			return m_sub_nodes[m_sub_nodes_index[s]].second;
		}



		// write
		inline static bool write(const Serializer& ar, std::ostream& fout, const std::string& indent = "\t", const char list_seperator = ',')
		{

			auto augmented_list_seperator = std::string(1, list_seperator) + " ";

			int indent_count = 0;

			std::function<void(const Serializer&, std::ostream&)> write_fn = //explicit type for recursive
				[&](const Serializer& ar, std::ostream& fout) 
			//auto write_fn = [&](const Serializer& ar, std::ostream& fout) 
			{

				auto indent_fn = [&](const std::string& s, const int count)
				{
					std::string out;
					for (int i = 0; i < count; i++)
					{
						out += s;
					}
					return out;
				};

				for (const auto& sub_node_it : ar.m_sub_nodes)
				{
					if (sub_node_it.second.m_sub_nodes.empty()) //no sub nodes
					{
						//write all contents
						fout << indent_fn(indent, indent_count) << sub_node_it.first << " = ";

						int list_size = sub_node_it.second.get_field_counts();
						for (int i = 0; i < list_size; i++)
						{
							auto x = sub_node_it.second.get_string(i).find_first_of(augmented_list_seperator);
							std::string sep = (list_size > 1 ? std::string{augmented_list_seperator} : "");
							if (x != std::string::npos)
							{
								fout << "\"" << sub_node_it.second.get_string(i) << "\"" << sep;
							}
							else
							{
								fout << sub_node_it.second.get_string(i) << sep;
							}

							list_size--;
						}

						fout << "\n";
					}
					else
					{

						fout << "\n" << indent_fn(indent, indent_count) << sub_node_it.first << " = ";
						fout << indent_fn(indent, indent_count) << "{\n";
						indent_count++;
						write_fn(sub_node_it.second, fout);
						fout << indent_fn(indent, indent_count) << "}\n\n";
					}
				}

				if (indent_count > 0)
				{
					indent_count--;
				}
			};


			//std::ofstream fout(output_file_name);
			if (fout)
			{
				write_fn(ar, fout);
				return true;
			}
			return false;

		}

		//read
		inline bool read(const Serializer& ar, const std::string& output_file_name, const std::string& indent = "\t", const char list_seperator = ',')
		{

		}

	private:
		std::vector<std::string> m_contents;
		std::vector<std::pair<std::string, Serializer>> m_sub_nodes;
		std::unordered_map<std::string, int> m_sub_nodes_index;
	};


	TEST(Serializer_Test,build_one)
	{
		Serializer s;

		s.set_string("hi");
		s.set_real(3.14, 1);
		s.set_int(2, 2);

		std::vector<std::string> exp = { "hi",std::to_string(3.14),std::to_string(2) };
		for (int i = 0; i < exp.size(); i++)
		{
			EXPECT_THAT(s.get_string(i), Eq(exp[i]));
		}

	}

	TEST(Serializer_Test,accses)
	{
		Serializer s;

		s["root_node"]["name"].set_string("Jack");
		s["root_node"]["age"].set_int(24);
		s["root_node"]["height"].set_real(3.14);

		s["root_node"]["works"].set_string("c++",0);
		s["root_node"]["works"].set_string("java",1);
		s["root_node"]["works"].set_string("c#", 2);


		EXPECT_THAT(s["root_node"]["works"].get_string(0), Eq("c++"));
		EXPECT_THAT(s["root_node"]["works"].get_string(1), Eq("java"));
		EXPECT_THAT(s["root_node"]["works"].get_string(2), Eq("c#"));


	}

	TEST(Serializer_Test,write)
	{
		Serializer s;

		s["root_node"]["name"].set_string("Jack");
		s["root_node"]["age"].set_int(24);
		s["root_node"]["height"].set_real(3.14);

		s["root_node"]["works"].set_string("c++",0);
		s["root_node"]["works"].set_string("java",1);
		s["root_node"]["works"].set_string("c#", 2);

		Serializer::write(s, std::cout);

		Serializer s1;

		s1.set_string("hi");
		s1.set_real(3.14, 1);
		s1.set_int(2, 2);

		Serializer::write(s1, std::cout);
	}

}

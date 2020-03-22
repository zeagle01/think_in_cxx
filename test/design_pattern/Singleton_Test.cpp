


#include "gmock/gmock.h"

#include <string>
#include <deque>
#include <memory>




#pragma once






namespace singleton_test
{


    class File_Repository
    {
        private:
            File_Repository() = default;
            std::deque<std::string> m_files;
            File_Repository(File_Repository &copy) = delete;

        public:
        ~File_Repository()
        {
            //printf("destroy!\n");
        }
            static File_Repository& get_singleton();

            void add_file(const std::string& file)
            {
                m_files.push_back(std::move(file));
            }

            void clear_files()
            {
                m_files.clear();
            }
            size_t get_file_size()
            {
                return m_files.size();
            }

    };

    File_Repository& File_Repository::get_singleton()
    {
        static auto instance = std::unique_ptr<File_Repository>{nullptr};
        if(!instance)
        {
            instance.reset(new File_Repository);
        }
        return *instance.get();
    }
}


using namespace testing;


class FileRepository_Test : public testing::Test
{


};




TEST_F(FileRepository_Test,test_add)
{
    //use reference
    auto &ins = singleton_test::File_Repository::get_singleton();
    ins.clear_files();
    ins.add_file("file1");
    ins.add_file("file2");
    ins.add_file("file3");
    EXPECT_THAT(ins.get_file_size(),Eq(3));
}

TEST_F(FileRepository_Test,test_clear)
{
    //use reference
    auto &ins = singleton_test::File_Repository::get_singleton();
    ins.clear_files();
    ins.add_file("file1");
    ins.clear_files();
    EXPECT_THAT(ins.get_file_size(),Eq(0));
}

#include "gtest/gtest.h"

#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include <iostream>

class FileIO
{
  public:
    FileIO(std::string &fileName)
    {
        connectFile(fileName);
    }

  private:
    void connectFile(const std::string &fileName)
    {
        try
        {
            mFout = std::make_shared<std::ofstream>(fileName);
            if (mFout&&mFout->good())
            {
                (*mFout) << "i'm here" << std::endl;
            }else{
                std::stringstream ss;
                ss << "can't open " << fileName << "!\n";
                ss<<"please type in  a valid file name !\n";
                throw std::runtime_error(ss.str());
            }
        }
        catch (std::runtime_error &e)
        {
            std::cout << e.what();
            std::string newFileName;
			return;
//            std::cin >> newFileName;
//            connectFile(newFileName);
        }
    }

//    std::shared_ptr<std::ifstream> mFin;
    std::shared_ptr<std::ofstream> mFout;
};

TEST(FileManipulationTest, test_use)
{

    std::string fileName="aasdf/fileIOquiz";
    FileIO fio(fileName);
    //FileIO fio("fileIOquiz");
}


#include "gmock/gmock.h"

#include <sstream>
#include <iostream>
#include <algorithm>
#include <string>

using namespace testing;


namespace dump_state
{

    class Record_IO
    {
        public:
        void dump(std::ostream& out,const std::vector<int>& v)
        {
            std::copy(std::begin(v),std::end(v),std::ostream_iterator<int>(out));
        };

        void load(std::istream& in,std::vector<int> &v)
        {
			v = std::vector <int>( std::istream_iterator<int>(in), std::istream_iterator<int>());
        }
    };

    struct State
    {
        std::vector<int> v0;
        std::vector<int> v1;

    };

//    static bool operator==(const State &left,const State &other)
//    {
//        if (left.v0 != other.v0)
//        {
//            return false;
//        }
//        if (left.v1 != other.v1)
//        {
//            return false;
//        }
//        return true;
//    }

    class State_IO
    {
        public:
            void dump(std::ostream &out, const State& state)
            {
                record_io.dump(out,state.v0);
                record_io.dump(out,state.v1);
            }

            void load(std::istream &in, State& state)
            {
                record_io.load(in,state.v0);
                record_io.load(in,state.v1);
            }

        private:
        Record_IO record_io;
    };
}




class Record_IO_Test:public testing::Test
{

protected:
    virtual void SetUp()
    {
        in_stream = std::istringstream(dump);
    }

protected:
    dump_state::Record_IO recored_io;

    std::ostringstream out_stream;

    std::istringstream in_stream;

    std::vector<int> data{1, 2, 3};

    std::string dump = "1 2 3";
};



TEST_F(Record_IO_Test,dump_array)
{

    recored_io.dump(out_stream,data);
    EXPECT_THAT(out_stream.str(),StrEq("123"));

}

TEST_F(Record_IO_Test,load_array)
{

    std::vector<int> act;
    recored_io.load(in_stream,act);
    EXPECT_EQ(act,data);

}



class A_State_That_Have_Two_Int_Vector:public testing::Test
{

protected:
    virtual void SetUp()
    {
        state.v0 = {1, 2, 3};
        state.v1 = {4, 5, 6};
        in_stream = std::istringstream(dump);
    }

protected:
    dump_state::State_IO state_io;

    std::ostringstream out_stream;

    std::istringstream in_stream;

    dump_state::State state;
    std::string dump = "3\n 1 2 3\n3\n4 5 6";
};



TEST_F(A_State_That_Have_Two_Int_Vector,test_dump)
{
    state_io.dump(out_stream,state);
    EXPECT_THAT(out_stream.str(),StrEq("123456"));

}

//TODO
TEST_F(A_State_That_Have_Two_Int_Vector,test_load)
{
    dump_state::State act;
    state_io.load(in_stream,act);
    EXPECT_EQ(act.v0,state.v0);
    EXPECT_EQ(act.v1,state.v1);

}


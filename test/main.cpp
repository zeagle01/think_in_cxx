
#include "gmock/gmock.h"
//#include "gtest/gtest.h"
#include <glm/vec3.hpp> // glm::vec3

int main(int argc,char** argv){

    glm::vec3 a;

    testing::InitGoogleMock(&argc,argv);
    return RUN_ALL_TESTS();
}

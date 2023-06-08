cmake_minimum_required(VERSION 3.20)


project(TMP_tutorial)


add_executable(TMP_tutorial main.cpp)

find_package(GTest CONFIG REQUIRED )
target_link_libraries(TMP_tutorial PUBLIC GTest::gmock_main)

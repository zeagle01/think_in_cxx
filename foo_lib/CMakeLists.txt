

cmake_minimum_required(VERSION 3.12)



project(foo)



add_library(foo print_hello.cpp)

target_include_directories(foo
PRIVATE ${CMAKE_SOURCE_DIR}/foo_lib/print_hello.h
)
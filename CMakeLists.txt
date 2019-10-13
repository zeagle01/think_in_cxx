

cmake_minimum_required(VERSION 3.12)

include(cmake/HunterGate.cmake)

HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.214.tar.gz"
    SHA1 "e14bc153a7f16d6a5eeec845fb0283c8fad8c358"
)


project(think_in_cxx)

add_executable(think_in_cxx 
    test/main.cpp
#    test/WriteFile.cpp
#    test/Pointer.cpp
#    test/TestConstructor.cpp
#    test/TestSharePointer.cpp
#    test/ECSSystemTest.cpp
#    test/Finally.cpp
#    test/Volatile.cpp
#    test/ExceptionTest.cpp
#    test/UnwindingTest.cpp
#    test/VariableTest.cpp
#    test/MySharedPtr.cpp
#    test/MultiThreadTest.cpp
#    test/OverloadTest.cpp
#    test/19_03_21.cpp
#    test/19_04_02.cpp
#    test/ProcessTest.cpp
#    test/FunctionReturnValueCauseCopy.cpp
    test/function_pointer.cpp
    test/BS/15_3_1.cpp
#    test/quiz/FileManipulationTest.cpp
#    test/quiz/ExceptionSafeTest.cpp
#    test/primer/C13/ActLikeValue.cpp
#    test/primer/C13/ActLikePointer.cpp
    test/primer/C13/MessageAndFolder.cpp
#     test/concurency/Chp2.cpp
#     test/concurency/Chp2_L0208.cpp
    )
target_compile_features(think_in_cxx PRIVATE cxx_std_11)



hunter_add_package(GTest)
find_package(GTest CONFIG QUIET)
target_link_libraries(think_in_cxx PRIVATE GTest::gtest)

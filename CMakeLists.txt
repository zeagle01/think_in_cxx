

cmake_minimum_required(VERSION 3.12)

include(cmake/HunterGate.cmake)

HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.214.tar.gz"
    SHA1 "e14bc153a7f16d6a5eeec845fb0283c8fad8c358"
)


project(think_in_cxx)

message("c++ compiler: ${CMAKE_CXX_COMPILER}")

add_executable(think_in_cxx 
    test/main.cpp
#    test/WriteFile.cpp
#    test/Pointer.cpp
#    test/TestConstructor.cpp
#    test/TestSharePointer.cpp
#    test/TestUniquePointer.cpp
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
#    test/function_pointer.cpp
#	 test/intro_2_alg/qsort_test.cpp
#    test/StdVectorTest.cpp
#    test/Cxx_Any_Test.cpp
#    test/Cxx_Optional_Test.cpp
#    test/Cxx_Variant_Test.cpp

### BS book
#    test/BS/15_3_1.cpp

### qt
#	 test/qt/qtHello.cpp

### Eigen
#	 test/EigenTest.cpp

#    test/quiz/FileManipulationTest.cpp
#    test/quiz/ExceptionSafeTest.cpp

### gtest
#	 test/gtest/GtestUsage.cpp
#	 test/gtest/gmock_test.cpp

### design pattern
#    test/design_pattern/State_Pattern_Test.cpp
#    test/design_pattern/Singleton_Test.cpp
#    test/design_pattern/Factory_Test.cpp
#    test/design_pattern/Builder_Test.cpp
#    test/design_pattern/Visitor_Test.cpp


###c++ primer
#    test/primer/C13/ActLikeValue.cpp
#    test/primer/C13/ActLikePointer.cpp
#    test/primer/C13/MessageAndFolder.cpp

#    test/concurency/Chp2.cpp
#    test/concurency/Chp2_L0208.cpp

###Jeff Langr
#	test/Jeff_Langr/c2/1/SoundexTest.cpp
#	test/Jeff_Langr/c2/1/Soundex.h
#	test/Jeff_Langr/c5/Http.h
#	test/Jeff_Langr/c5/PlaceDescriptionServiceTest.cpp


###dump state
	test/dump_state/Dump_State_Test.cpp

    )
target_compile_features(think_in_cxx PRIVATE cxx_std_17)



#hunter_add_package(GTest)
#find_package(GMock CONFIG QUIET)
#target_link_libraries(think_in_cxx PRIVATE GMock::gmock)

################### get newest gtest###################
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        master
)

# this one is important for windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)  

FetchContent_MakeAvailable(googletest)

#FetchContent_GetProperties(googletest)
#if(NOT googletest_POPULATED)
#  FetchContent_Populate(googletest)
#  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
#endif()

set_target_properties(gmock_main PROPERTIES EXCLUDE_FROM_ALL TRUE)

target_link_libraries(think_in_cxx PRIVATE gmock_main)
################### get newest gtest###################

hunter_add_package(Eigen)
find_package(Eigen3 CONFIG REQUIRED)
target_link_libraries(think_in_cxx PRIVATE Eigen3::Eigen)


set(Qt5_DIR "D:/Qt/5.13.1/msvc2017_64/lib/cmake/Qt5/" CACHE PATH "qt5 cmake path")
set(Qt5_components Core Widgets Gui)
set(Qt5_link_components)
foreach(c ${Qt5_components})
	list(APPEND Qt5_link_components Qt5::${c})
endforeach()

find_package(Qt5 COMPONENTS ${Qt5_components} REQUIRED) 
target_link_libraries(think_in_cxx PRIVATE ${Qt5_link_components})

if(MSVC)
	foreach(c ${Qt5_link_components})
		add_custom_command(
			TARGET think_in_cxx POST_BUILD
			#COMMAND ${CMAKE_COMMAND} -E create_symlink
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				$<TARGET_FILE:${c}>
				$<TARGET_FILE_DIR:think_in_cxx>
		)
	endforeach()
endif()


source_group("cxx" FILES "test/WriteFile.cpp")
source_group("cxx" FILES "test/Pointer.cpp")
source_group("cxx" FILES "test/TestConstructor.cpp")
source_group("cxx" FILES "test/TestSharePointer.cpp")
source_group("cxx" FILES "test/Finally.cpp")
source_group("cxx" FILES "test/Volatile.cpp")
source_group("cxx" FILES "test/ExceptionTest.cpp")
source_group("cxx" FILES "test/VariableTest.cpp")
source_group("cxx" FILES "test/MySharedPtr.cpp")
source_group("cxx" FILES "test/MultiThreadTest.cpp")
source_group("cxx" FILES "test/OverloadTest.cpp")
source_group("cxx" FILES "test/19_03_21.cpp")
source_group("cxx" FILES "test/19_04_02.cpp")
source_group("cxx" FILES "test/ProcessTest.cpp")
source_group("cxx" FILES "test/FunctionReturnValueCauseCopy.cpp")
source_group("cxx" FILES "test/function_pointer.cpp")
source_group("cxx" FILES "test/15_3_1.cpp")
source_group("cxx" FILES "test/FileManipulationTest.cpp")
source_group("cxx" FILES "test/ExceptionSafeTest.cpp")
source_group("cxx" FILES "test/ProcessTest.cpp")
source_group("cxx" FILES "test/StdVectorTest.cpp")

source_group("qt" FILES "test/qt/qtHello.cpp")

source_group("eigen" FILES "test/EigenTest.cpp")

source_group("intro_2_alg" FILES "test/intro_2_alg/qsort_test.cpp")

source_group("primer" FILES "test/primer/C13/ActLikeValue.cpp")
source_group("primer" FILES "test/primer/C13/ActLikePointer.cpp")
source_group("primer" FILES "test/primer/C13/MessageAndFolder.cpp")

source_group("concurency" FILES "test/concurency/Chp2.cpp")
source_group("concurency" FILES "test/concurency/Chp2_L0208.cpp")

source_group("gtest" FILES "test/GtestUsage.cpp")

source_group("Jeff_Langr" FILES "test/c2/1/SoundexTest.cpp")
source_group("Jeff_Langr" FILES "test/c2/1/Soundex.cpp")
source_group("Jeff_Langr" FILES "test/c5/Http.cpp")
source_group("Jeff_Langr" FILES "test/c5/PlaceDescriptionServiceTest.cpp")





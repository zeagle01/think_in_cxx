

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
#    test/BS/15_3_1.cpp
#    test/quiz/FileManipulationTest.cpp
#    test/quiz/ExceptionSafeTest.cpp
#	test/qt/qtHello.cpp
#	test/EigenTest.cpp
#	test/intro_2_alg/qsort_test.cpp
#	test/StdVectorTest.cpp

###c++ primer
#    test/primer/C13/ActLikeValue.cpp
#    test/primer/C13/ActLikePointer.cpp
#    test/primer/C13/MessageAndFolder.cpp

#     test/concurency/Chp2.cpp
#     test/concurency/Chp2_L0208.cpp
#	test/GtestUsage.cpp

###Jeff Langr
	test/Jeff_Langr/c2/1/SoundexTest.cpp
	test/Jeff_Langr/c2/1/Soundex.h

#	test/qt/qtHello.cpp
#	test/EigenTest.cpp
#	test/intro_2_alg/qsort_test.cpp
#	test/StdVectorTest.cpp
	test/MyMatrixMath.cpp
    )
target_compile_features(think_in_cxx PRIVATE cxx_std_11)



hunter_add_package(GTest)
#find_package(GTest CONFIG QUIET)
find_package(GMock CONFIG QUIET)
#target_link_libraries(think_in_cxx PRIVATE GTest::gtest GMock::gmock)
target_link_libraries(think_in_cxx PRIVATE GMock::gmock)


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

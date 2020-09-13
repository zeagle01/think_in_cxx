

cmake_minimum_required(VERSION 3.12)

#include(cmake/HunterGate.cmake)
#
#HunterGate(
#    URL "https://github.com/cpp-pm/hunter/archive/v0.23.265.tar.gz"
#    SHA1 "eae0bb5b7b8aa968e0ff87ec51be51b4255db110"
#	#LOCAL
#)

project(think_in_cxx)

message("c++ compiler: ${CMAKE_CXX_COMPILER}")

add_executable(think_in_cxx 
	test/main.cpp
######language
#    test/lang/WriteFile.cpp
#    test/lang/Pointer.cpp
#    test/lang/TestConstructor.cpp
#    test/lang/TestSharePointer.cpp
#    test/lang/TestUniquePointer.cpp
#    test/lang/ECSSystemTest.cpp
#    test/lang/Finally.cpp
#    test/lang/Volatile.cpp
#    test/lang/ExceptionTest.cpp
#    test/lang/UnwindingTest.cpp
#    test/lang/VariableTest.cpp
#    test/lang/MySharedPtr.cpp
#    test/lang/MultiThreadTest.cpp
#    test/lang/OverloadTest.cpp
#    test/lang/19_03_21.cpp
#    test/lang/19_04_02.cpp
#    test/lang/ProcessTest.cpp
#    test/lang/FunctionReturnValueCauseCopy.cpp
#    test/lang/function_pointer.cpp
#	 test/inlang/tro_2_alg/qsort_test.cpp
#    test/lang/StdVectorTest.cpp
#    test/lang/StdMapTest.cpp
#    test/lang/Std_Set_Test.cpp
#    test/lang/Cxx_Any_Test.cpp
#    test/lang/Cxx_Optional_Test.cpp
#    test/lang/Cxx_Variant_Test.cpp
#    test/lang/Subclass_Visit_Supclass.cpp
#    test/lang/variadic_template.cpp
#    test/lang/template_concept.cpp
#    test/lang/template_crtp.cpp
#    test/lang/show_bytes.cpp
#    test/lang/use_foo.cpp #TO MOVE TO BUILD
#    test/lang/random.cpp 
#    test/lang/chrono_test.cpp 
#    test/lang/static_reflection.cpp  #TO BE VERIFY ,Not work in windows
#    test/lang/refer_class_member_data.cpp  
#    test/lang/virtual_non_virtual.cpp  
#    test/lang/concept_test.cpp  



### tdd
#    test/little_projects/show_hand.cpp  ##TODO


### design
#    test/design/dispatch_member.cpp 
    test/design/runtime_polymorphism.cpp  #sean parent demo

### tools
#    test/tools/profiler.cpp 


### template_meta
#    test/template_meta/compendium.cpp
#    test/template_meta/static_loop.cpp
#    test/template_meta/event_dispatch.cpp

### alg
#    test/alg/binary_tree.cpp
#    test/alg/bvh.cpp #TODO
#    test/alg/sort.cpp #TODO


### BS book
#    test/BS/15_3_1.cpp

### qt
#	 test/qt/qtHello.cpp

### Eigen
#	 test/eigen/EigenTest.cpp

#    test/quiz/FileManipulationTest.cpp
#    test/quiz/ExceptionSafeTest.cpp

### gtest
#	 test/gtest/GtestUsage.cpp
#	 test/gtest/gmock_test.cpp
#	 test/gtest/parameter_test.cpp
#	 test/gtest/type_parameter_test.cpp

### design pattern
#    test/design_pattern/State_Pattern_Test.cpp
#    test/design_pattern/Singleton_Test.cpp
#    test/design_pattern/Factory_Test.cpp
#    test/design_pattern/Builder_Test.cpp
#    test/design_pattern/Visitor_Test.cpp 
#    test/design_pattern/acyclic_visitor.cpp ##TODO
#    test/design_pattern/Open_Closed_Principle_Test.cpp
#    test/design_pattern/Observer_Test.cpp
#    test/design_pattern/Type_Erasure_Test.cpp


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


###Packt fp
#	test/Packt/tic_tac_toe.cpp


###dump state
#	test/dump_state/Dump_State_Test.cpp

    )
target_compile_features(think_in_cxx PRIVATE cxx_std_20)



#hunter_add_package(GTest)
#find_package(GMock CONFIG QUIET)
#target_link_libraries(think_in_cxx PRIVATE GMock::gmock)

#hunter_add_package(Eigen)
#find_package(Eigen3 CONFIG REQUIRED)
#target_link_libraries(think_in_cxx Eigen3::Eigen)

################### get newest gtest###################
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY ${CMAKE_SOURCE_DIR}/3rd/gtest
  GIT_TAG        master
)

# this one is important for windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)  

FetchContent_MakeAvailable(googletest)

set_target_properties(gmock_main PROPERTIES EXCLUDE_FROM_ALL TRUE)
target_link_libraries(think_in_cxx PRIVATE gmock_main)
################### get newest gtest###################

##################eigen
FetchContent_Declare(
  eigen_repo
  GIT_REPOSITORY ${CMAKE_SOURCE_DIR}/3rd/eigen
  GIT_TAG        master
)

FetchContent_MakeAvailable(eigen_repo)
## turn off build testing option in UI
target_link_libraries(think_in_cxx PRIVATE Eigen3::Eigen)
##################eigen


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



target_link_libraries(think_in_cxx PRIVATE foo)

add_subdirectory(foo_lib)







cmake_minimum_required(VERSION 3.23)


project(think_in_cxx)

message("c++ compiler: ${CMAKE_CXX_COMPILER}")

add_executable(think_in_cxx 
	test/main.cpp
######language
#    test/lang/WriteFile.cpp
#    test/lang/Pointer.cpp
#    test/lang/TestConstructor.cpp
    test/lang/TestSharePointer.cpp
    test/lang/reference.cpp
    test/lang/special_methods.cpp
#    test/lang/TestUniquePointer.cpp
#    test/lang/ECSSystemTest.cpp
#    test/lang/Finally.cpp
#    test/lang/Volatile.cpp
#    test/lang/ExceptionTest.cpp
#    test/lang/UnwindingTest.cpp
#    test/lang/VariableTest.cpp
    test/lang/MySharedPtr.cpp
    test/lang/vector.cpp
#    test/lang/MultiThreadTest.cpp
#    test/lang/OverloadTest.cpp
#    test/lang/19_03_21.cpp
#    test/lang/19_04_02.cpp
#    test/lang/ProcessTest.cpp
#    test/lang/FunctionReturnValueCauseCopy.cpp
#    test/lang/function_pointer.cpp
#    test/inlang/tro_2_alg/qsort_test.cpp
#    test/lang/StdVectorTest.cpp
    test/lang/std_list_test.cpp
    test/lang/StdMapTest.cpp
#    test/lang/Std_Set_Test.cpp
#    test/lang/Cxx_Any_Test.cpp
#    test/lang/Cxx_Optional_Test.cpp
    test/lang/Cxx_Variant_Test.cpp
#    test/lang/Subclass_Visit_Supclass.cpp
#    test/lang/variadic_template.cpp
    test/lang/deduction_guide.cpp
    test/lang/concept_test.cpp
#    test/lang/template_crtp.cpp
#    test/lang/show_bytes.cpp
#    test/lang/use_foo.cpp #TO MOVE TO BUILD
    test/lang/random.cpp 
#    test/lang/chrono_test.cpp 
    test/lang/static_reflection.cpp  
#    test/lang/refer_class_member_data.cpp  
#    test/lang/virtual_non_virtual.cpp  
    test/lang/macro.cpp  
    test/lang/module.cpp


### cxx23
    test/cxx23/deducing_this.cpp  

### tdd
#    test/little_projects/show_hand.cpp  ##TODO


### design
#    test/design/dispatch_member.cpp 
#    test/design/runtime_polymorphism.cpp  #sean parent demo
    test/design/maybe_monad.cpp  #
    test/design/product_factory.cpp  #
		test/design/product_factory/product.h  #
		test/design/product_factory/concrete_product.h  #
		test/design/product_factory/concrete_product.cpp  #

### tools
#    test/tools/profiler.cpp 

### data_structure
    test/data_structure/heap.cpp 


### template_meta
    test/template_meta/type_deduction.cpp
#    test/template_meta/compendium.cpp
#    test/template_meta/static_loop.cpp
#    test/template_meta/event_dispatch.cpp
    test/template_meta/type_list.cpp
#    test/template_meta/multi_method.cpp
    test/template_meta/loop_enum.cpp
    test/template_meta/type_map.cpp
    test/template_meta/loop_types.cpp
    test/template_meta/string_parameter.cpp
    test/template_meta/partial_specalization_to_dispatch.cpp

### alg
#    test/alg/binary_tree.cpp
#    test/alg/bvh.cpp #TODO
    test/alg/sort.cpp #TODO


### BS book
#    test/BS/15_3_1.cpp

### qt
#	 test/qt/qtHello.cpp

### Eigen
#	 test/eigen/EigenTest.cpp

### quiz
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
    test/design_pattern/Visitor_Test.cpp 
    test/design_pattern/acyclic_visitor.cpp ##TODO
#    test/design_pattern/Open_Closed_Principle_Test.cpp
    test/design_pattern/Observer_Test.cpp
    test/design_pattern/Type_Erasure_Test.cpp
#    test/design_pattern/signal_slot_test.cpp
    test/design_pattern/value_polymorphism.cpp


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
#	test/functional/maybe.cpp


###dump state
#	test/dump_state/Dump_State_Test.cpp

### cuda
	test/cuda/function_pointer.cu
	test/cuda/template_kernel.cu

### threading
	test/threading/basic_use.cpp
	test/threading/condition_variable.cpp
	test/threading/thread_pool.cpp

###  tbb
	test/tbb/tbb_test.cpp
	#test/tbb/parallel_log.cpp
### serializer
	test/serializer/serializer_test.cpp

    )

###moduel support
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)

# UUID at https://gitlab.kitware.com/cmake/cmake/-/blob/master/Help/dev/experimental.rst
if(CMAKE_VERSION VERSION_LESS 3.26)
  # 3.25.2...3.26
  set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "3c375311-a3c9-4396-a187-3227ef642046")
elseif(CMAKE_VERSION VERSION_LESS 3.27)
  set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
elseif(CMAKE_VERSION VERSION_LESS 3.28)
  set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "aa1f7df0-828a-4fcd-9afc-2dc80491aca7")
elseif(CMAKE_VERSION VERSION_LESS 3.29)
  set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "a816ed09-43d1-40e5-bc8c-1a2824ee194e")
endif()

target_sources(think_in_cxx PUBLIC
	FILE_SET cxx_modules
	TYPE CXX_MODULES
	FILES test/lang/module/math.ixx
)


target_compile_features(think_in_cxx PRIVATE cxx_std_23)





####### eigen ########
find_package(Eigen3 CONFIG REQUIRED)
target_link_libraries(think_in_cxx PUBLIC Eigen3::Eigen)

####### gtest ########
find_package(GTest CONFIG REQUIRED )
target_link_libraries(think_in_cxx PUBLIC GTest::gmock_main)



####### glm ########
find_package(glm CONFIG REQUIRED)
target_link_libraries(think_in_cxx PUBLIC glm::glm)

####### tbb ########
find_package(TBB CONFIG REQUIRED)
target_link_libraries(think_in_cxx PUBLIC TBB::tbb)

####### qt ########
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



####### foo ########
target_link_libraries(think_in_cxx PRIVATE foo)

add_subdirectory(foo_lib)
add_subdirectory(playground/module_demo)


####### cuda ########
include(FindCUDAToolkit)
if(CUDAToolkit_FOUND)
	message("CUDAToolkit_BIN_DIR: ${CUDAToolkit_BIN_DIR}")
	message("CUDAToolkit_INCLUDE_DIRS: ${CUDAToolkit_INCLUDE_DIRS}")
	message("CUDAToolkit_LIBRARY_DIR: ${CUDAToolkit_LIBRARY_DIR}")
	message("CUDAToolkit_TARGET_DIR: ${CUDAToolkit_TARGET_DIR}")
	message("CUDAToolkit_NVCC_EXECUTABLE: ${CUDAToolkit_NVCC_EXECUTABLE}")

	enable_language(CUDA)
endif()


####### omp ########
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
	message("omp found!")
    target_link_libraries(think_in_cxx PUBLIC OpenMP::OpenMP_CXX)
else()
	message("omp not found!")
endif()


##copy dlls
add_custom_command(TARGET think_in_cxx POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_RUNTIME_DLLS:think_in_cxx> $<TARGET_FILE_DIR:think_in_cxx> 
	COMMAND_EXPAND_LISTS
)

add_custom_command(TARGET think_in_cxx POST_BUILD COMMAND ${CMAKE_COMMAND} -E echo 
	"copy dlls: $<TARGET_RUNTIME_DLLS:think_in_cxx> -> $<TARGET_FILE_DIR:think_in_cxx> "
)



########################## playground projects ##########################

### spinning cube 
add_executable(spinning_cube playground/spinning_cube/main.cpp)
target_compile_features(spinning_cube PRIVATE cxx_std_20)

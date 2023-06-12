cmake_minimum_required(VERSION 3.20)


project(TMP_tutorial)


add_executable(
	TMP_tutorial 
	main.cpp
	meta_function_test.cpp
	loop_test.cpp
	type_list_test.cpp
)


target_compile_features(TMP_tutorial PRIVATE cxx_std_20)

find_package(GTest CONFIG REQUIRED )
target_link_libraries(TMP_tutorial PUBLIC GTest::gmock_main)


##copy dlls
add_custom_command(TARGET TMP_tutorial POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_RUNTIME_DLLS:TMP_tutorial> $<TARGET_FILE_DIR:TMP_tutorial> 
	COMMAND_EXPAND_LISTS
)

add_custom_command(TARGET TMP_tutorial POST_BUILD COMMAND ${CMAKE_COMMAND} -E echo 
	"copy dlls: $<TARGET_RUNTIME_DLLS:TMP_tutorial> -> $<TARGET_FILE_DIR:TMP_tutorial> "
)

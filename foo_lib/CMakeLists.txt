

cmake_minimum_required(VERSION 3.12)



project(foo)



add_library(foo SHARED
src/print_hello.cpp)

target_include_directories(foo
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
PRIVATE
	src
)


#set_target_properties(JSONUtils::JSONUtils PROPERTIES
#  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/include"
#  INTERFACE_LINK_LIBRARIES "Boost::boost;RapidJSON::RapidJSON;\$<LINK_ONLY:Boost::regex>"
#)




include(GNUInstallDirs)
#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES CACHE BOOL "Export all symbols")
install(
#    TARGETS <project>_<target>_1 <project>_<target>_2
#    EXPORT <project>Targets
    TARGETS foo
    EXPORT foo_targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # this is for windows
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/foo)
#install(FILE ${CMAKE_CURRENT_SOURCE_DIR}/include/print_hello.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/foo)

# This makes the project importable from the install directory
# Put config file in per-project dir (name MUST match), can also
# just go into 'cmake'.
install(EXPORT foo_targets 
FILE foo_targets.cmake
DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/foo
)

# This makes the project importable from the build directory
 export(TARGETS foo FILE foo_targets.cmake)




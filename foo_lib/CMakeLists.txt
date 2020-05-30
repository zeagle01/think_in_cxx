

cmake_minimum_required(VERSION 3.12)



project(foo)


###except global variable like static class member
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES CACHE BOOL "Export all symbols")


add_library(foo SHARED
src/print_hello.cpp
include/print_hello.h
)

target_include_directories(foo
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
PRIVATE
	${CMAKE_CURRENT_BINARY_DIR}## for export headers
	src
)



include(GNUInstallDirs)
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









# This makes the project importable from the install directory
# Put config file in per-project dir (name MUST match), can also
# just go into 'cmake'.
install(EXPORT foo_targets 
FILE foo_targets.cmake
DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/foo
)

####version specify
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    foo_targets.cmake
    VERSION 1.0.0
    COMPATIBILITY AnyNewerVersion
    )


# This makes the project importable from the build directory
 export(TARGETS foo FILE foo_targets.cmake)



######### generate export dll symbols headers
include(GenerateExportHeader)
generate_export_header(foo)


######## out put in same directory so that dll can be used by exe
set_target_properties( think_in_cxx foo
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/bin
)
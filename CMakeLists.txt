# GraphX

cmake_minimum_required(VERSION 3.16..3.24)

# Project configuration
project(
    WFC
    VERSION 0.0.1
    DESCRIPTION "Wave Function Collapse algorithm written in C++"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD              17)
set(CMAKE_CXX_STANDARD_REQUIRED     TRUE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_SOURCE_DIR}/bin)

# Set the default value of BUILD_TESTING to ON
option(BUILD_TESTING "Build tests" ON)

# Update the submodules here
include(cmake/UpdateSubmodules.cmake)

# Add include directory for the library
include_directories(${CMAKE_SOURCE_DIR}/src)

# Fetch Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.tar.gz
)
FetchContent_MakeAvailable(googletest)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(
        docs
        ${DOXYGEN_EXECUTABLE}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs)
endif()

# Add subdirectories with code
add_subdirectory(src)
add_subdirectory(test)
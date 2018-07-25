cmake_minimum_required (VERSION 3.10)

# Define the project
project(process_lib VERSION 1.0.0 DESCRIPTION "process library for easily extending your program with more control")

# File globbing
file(GLOB_RECURSE sources src/*.cpp src/*.hpp src/impl/*.cpp src/impl/*.hpp src/include/*.cpp src/include/*.hpp src/include/*.c src/include/*.h)
#file(GLOB_RECURSE sources_test src/test/*.cpp src/test/*.hpp)

# Add definition for process library
add_library(process_lib SHARED ${sources})

set_target_properties(process_lib PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(process_lib PROPERTIES PUBLIC_HEADER src/include/process.hpp)

# add CXX compiler options
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11" )

# add more compiler flags.
target_compile_options(process_lib PUBLIC -Wall)

# include files
target_include_directories(process_lib PUBLIC ${CMAKE_BINARY_DIR}/src/include /usr/include)

# Third-party library
include(ExternalProject)
ExternalProject_Add(googletest
    PREFIX "${CMAKE_BINARY_DIR}/lib"
    GIT_REPOSITORY "https://github.com/google/googletest.git"
    GIT_TAG "master"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/lib/installed
)

include(ExternalProject)
ExternalProject_Add(g3log
    PREFIX "${CMAKE_BINARY_DIR}/lib"
    GIT_REPOSITORY "https://github.com/KjellKod/g3log.git"
    GIT_TAG "1.3"
    CMAKE_ARGS -DUSE_DYNAMIC_LOGGING_LEVELS=ON -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/lib/installed
)

# Prevent build on all targets build
set_target_properties(googletest PROPERTIES EXCLUDE_FROM_ALL TRUE)
set_target_properties(g3log PROPERTIES EXCLUDE_FROM_ALL TRUE)


# Define ${CMAKE_INSTALL_...} variables
include(GNUInstallDirs)

# Specify where third-party libraries are located
link_directories(${CMAKE_BINARY_DIR}/lib/installed/${CMAKE_INSTALL_LIBDIR})
include_directories(${CMAKE_BINARY_DIR}/lib/installed/${CMAKE_INSTALL_INCLUDEDIR})

# This is required by library
#find_package(Boost 1.65.1 COMPONENTS system asio REQUIRED)
# This is required for googletest
find_package(Threads REQUIRED)

# Test
#add_executable(process_test test/*.cpp test/*.hpp)
#target_link_libraries(process_test g3log gtest Threads::Threads process_lib )

# Make sure third-party is built before executable
add_dependencies(process_lib g3log)
#add_dependencies(process_lib Boost)

#add_dependencies(process_test googletest)
#add_dependencies(process_test g3log)
#set_target_properties(process_test PROPERTIES EXCLUDE_FROM_ALL TRUE)

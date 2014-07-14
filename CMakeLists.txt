cmake_minimum_required(VERSION 2.8.3)
project(quat)

# need google test and eigen
find_package(Eigen3 REQUIRED)

enable_testing()

message(STATUS "Using gtest directory: $ENV{GTEST_DIR}")

include_directories(include)
include_directories($ENV{GTEST_DIR}/include)
include_directories(${Eigen3_INCLUDE_DIR})

# Compile test suite
add_definitions("-std=c++11")

# gtest
link_directories($ENV{GTEST_DIR}/lib/.libs)
add_executable(${PROJECT_NAME}_tests test/quaternion_test.cpp)
target_link_libraries(${PROJECT_NAME}_tests gtest gtest_main pthread)

add_test(
  NAME unitTests
  COMMAND ${PROJECT_NAME}_tests
)

cmake_minimum_required(VERSION 2.8.3)
project(quat)

# need google test and eigen
find_package(Eigen3 REQUIRED)

enable_testing()

# Find gtest either via environment variable GTEST_DIR or by supplying a hint
# directory GTEST_HINTS
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(GTEST_HINTS $ENV{GTEST_DIR})
if(NOT GTEST_HINTS OR NOT EXISTS ${GTEST_HINTS})
    set(GTEST_HINTS /opt/gtest-1.7.0)
endif(NOT GTEST_HINTS OR NOT EXISTS ${GTEST_HINTS})
find_package(gtest REQUIRED)

include_directories(include)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${Eigen3_INCLUDE_DIR})

# Compile test suite
add_definitions("-std=c++11")

# gtest
add_executable(${PROJECT_NAME}_tests test/quaternion_test.cpp)
target_link_libraries(${PROJECT_NAME}_tests ${GTEST_LIBRARIES} pthread)

add_test(
    NAME unitTests
    COMMAND ${PROJECT_NAME}_tests
)

cmake_minimum_required(VERSION 2.8.3)
project(quat)

# need google test and eigen
find_package(Eigen3 REQUIRED)

enable_testing()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(GTEST_HINTS /opt/gtest-1.7.0)
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

cmake_minimum_required(VERSION 3.10)
project(QueueLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(queue_lib INTERFACE)
target_include_directories(queue_lib INTERFACE include)

add_executable(demo src/main.cpp)
target_link_libraries(demo queue_lib)

add_subdirectory(gtest)

add_executable(tests tests/test_queue.cpp)
target_link_libraries(tests queue_lib gtest gtest_main pthread)

enable_testing()
add_test(NAME tests COMMAND tests)

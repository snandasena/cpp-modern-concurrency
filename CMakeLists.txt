cmake_minimum_required(VERSION 3.22)
project(CppConcurrency)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-fsanitize=address -ggdb3 -Wall  -ansi  -Wextra -Wpedantic")

#add_executable(JthreadDemo src/jthread_demo.cpp)
#target_link_libraries(JthreadDemo -pthread)

add_executable(YieldDemo src/yield_demo.cpp)
target_link_libraries(YieldDemo -pthread)
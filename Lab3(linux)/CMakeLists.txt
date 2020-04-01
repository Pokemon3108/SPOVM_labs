cmake_minimum_required(VERSION 3.15)
project(Lab3)

set(CMAKE_CXX_STANDARD 14)
find_package(Threads REQUIRED)
SET(CMAKE_CXX_FLAGS -pthread)
add_executable(Lab3 main.cpp)
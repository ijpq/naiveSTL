cmake_minimum_required(VERSION 3.10)
project(TestCPPProject)

# 设置全局 C++ 标准（此处以 C++17 为例，你可以根据需要改为 C++11、C++20 等）
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_executable(main main.cpp)
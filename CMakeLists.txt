cmake_minimum_required(VERSION 3.10)
project(TestCPPProject)

# 设置全局 C++ 标准（此处以 C++17 为例，你可以根据需要改为 C++11、C++20 等）
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable ASan
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

# COMPILE_COMMANDS
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory(test)
# file( GLOB CHILDREN RELATIVE ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/*)

# foreach(CH ${CHILDREN}) # 拼出绝对路径 set(CH_PATH "${CMAKE_SOURCE_DIR}/${CH}") # 如果是一个目录
# if(IS_DIRECTORY ${CH_PATH}) # 过滤掉某些特殊目录 if(NOT CH MATCHES "^\\.git$" AND NOT CH MATCHES "^build$")
# # 检查是否存在 CMakeLists.txt if(EXISTS "${CH_PATH}/CMakeLists.txt") message(STATUS "  ➜ 添加子目录：${CH}")
# add_subdirectory(${CH}) else() message(STATUS "  ⚠️ 跳过子目录（缺少 CMakeLists.txt）：${CH}") endif()
# endif() endif() endforeach()

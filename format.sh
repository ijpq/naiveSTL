#!/usr/bin/env bash
#
# format_all.sh
#
# 使用 clang-format 对当前项目中所有 C/C++ 源文件进行格式化，
# 使用 cmake-format 对当前项目中所有 CMakeLists.txt 及 *.cmake 文件进行格式化，
# 并忽略名为 "build" 的目录。
#
# 依赖：
#   - clang-format：用于格式化 .cpp/.h 等 C/C++ 文件
#   - cmake-format：用于格式化 CMakeLists.txt 和 *.cmake 文件
#
# 用法：
#   将本脚本放在项目根目录，赋予可执行权限（chmod +x format_all.sh），
#   然后在项目根执行：./format_all.sh
#

set -euo pipefail

echo "=========================================="
echo "开始格式化 C/C++ 以及 CMake 文件（忽略 build 目录）..."
echo "=========================================="

###############################################################################
# 1. 格式化所有 C/C++ 相关文件（忽略 build 目录）
###############################################################################

echo ">> 格式化 C/C++ 文件（.cpp/.cc/.cxx/.c/.hpp/.h/.hh/.hxx）："

find . \
    -type d -name build -prune -o \
    -type f \( \
        -iname '*.cpp'  \
     -o -iname '*.cc'   \
     -o -iname '*.cxx'  \
     -o -iname '*.c'    \
     -o -iname '*.hpp'  \
     -o -iname '*.h'    \
     -o -iname '*.hh'   \
     -o -iname '*.hxx'  \
    \) -print0 | while IFS= read -r -d '' file; do
    echo "  ↪ 正在格式化：$file"
    clang-format -i "$file"
done

echo "✔ C/C++ 文件格式化完成。"
echo

###############################################################################
# 2. 格式化所有 CMake 相关文件（忽略 build 目录）
###############################################################################

echo ">> 格式化 CMake 文件（CMakeLists.txt/*.cmake）："

find . \
    -type d -name build -prune -o \
    -type f \( \
        -iname 'CMakeLists.txt' \
     -o -iname '*.cmake'         \
    \) -print0 | while IFS= read -r -d '' file; do
    echo "  ↪ 正在格式化：$file"
    cmake-format -i "$file"
done

echo "✔ CMake 文件格式化完成。"
echo

echo "=========================================="
echo "全部格式化操作已完成。"
echo "=========================================="

#!/usr/bin/env bash
#
# format_all.sh - 格式化项目中的 C/C++ 和 CMake 文件（支持 dry run）
#
# 用法：
#   ./format_all.sh           # Dry run：显示差异但不修改文件
#   ./format_all.sh -i        # 实际修改文件
#

set -euo pipefail

DRY_RUN=true

# 参数解析
for arg in "$@"; do
  case "$arg" in
    -i|--in-place)
      DRY_RUN=false
      ;;
    *)
      echo "未知参数：$arg"
      echo "用法: $0 [-i|--in-place]"
      exit 1
      ;;
  esac
done

echo "=========================================="
if $DRY_RUN; then
  echo "Dry run 模式：显示格式化 diff，但不修改文件。"
else
  echo "In-place 模式：将直接修改文件。"
fi
echo "=========================================="

###############################################################################
# 1. 格式化 C/C++ 文件
###############################################################################

echo ">> C/C++ 文件（.cpp/.cc/.cxx/.c/.hpp/.h/.hh/.hxx）："

find . \
    -type d -name build -prune -o \
    -type f \( \
        -iname '*.cpp'  -o -iname '*.cc'   -o -iname '*.cxx' \
     -o -iname '*.c'    -o -iname '*.hpp'  -o -iname '*.h' \
     -o -iname '*.hh'   -o -iname '*.hxx' \
    \) -print0 | while IFS= read -r -d '' file; do
    echo "  ↪ 检查/格式化：$file"
    if $DRY_RUN; then
        tmpfile=$(mktemp)
        clang-format "$file" > "$tmpfile"
        if ! diff -q "$file" "$tmpfile" > /dev/null; then
            echo "--- 格式化 diff（彩色） ---"
            git diff --no-index --color=always "$file" "$tmpfile" || true
            echo
        fi
        rm -f "$tmpfile"
    else
        clang-format -i "$file"
    fi
done

echo "✔ C/C++ 文件处理完成。"
echo

###############################################################################
# 2. 格式化 CMake 文件
###############################################################################

echo ">> CMake 文件（CMakeLists.txt/*.cmake）："

find . \
    -type d -name build -prune -o \
    -type f \( -iname 'CMakeLists.txt' -o -iname '*.cmake' \) -print0 | while IFS= read -r -d '' file; do
    echo "  ↪ 检查/格式化：$file"
    if $DRY_RUN; then
        tmpfile=$(mktemp)
        cmake-format "$file" > "$tmpfile"
        if ! diff -q "$file" "$tmpfile" > /dev/null; then
            echo "--- 格式化 diff ---"
            git diff --no-index --color=always "$file" "$tmpfile" || true
            echo
        fi
        rm -f "$tmpfile"
    else
        cmake-format -i "$file"
    fi
done

echo "✔ CMake 文件处理完成。"
echo

echo "=========================================="
echo "全部操作已完成。"
echo "=========================================="

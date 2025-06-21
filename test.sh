
rm -rf ./build && mkdir build && \
cd build && \
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo && \
make -j2 && \

export ASAN_OPTIONS=detect_leaks=1

# 查找 test/ 目录下所有无扩展名的可执行文件并执行
find ./test -type f -executable ! -name "*.*" | sort | while read -r test_exec; do
    echo "Running $test_exec"
    "$test_exec"
done

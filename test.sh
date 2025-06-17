rm -rf ./build && mkdir build && \
cd build && \
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo &&\
make -j 2 && \
ASAN_OPTIONS=detect_leaks=1 ./test/allocator/allocator

#include <allocator.h>

static constexpr int size = 10;
int                  main() {
    nstd::allocator<int> alloc;
    alloc.allocate(10);
    alloc.allocate(size);
    return 0;
}
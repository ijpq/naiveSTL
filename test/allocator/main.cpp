// tests/test_vector.cpp
#include "allocator.h"
#include "core/compressed_pair.h"
#include "vector.h" // 假设你的实现位于 nstd 命名空间
#include <cassert>
#include <iostream>
#include <string>

using namespace nstd;

// TODO
// struct ThrowOnN {
//     static int count;
//     ThrowOnN() {
//         if (++count == 5) throw std::runtime_error("fail");
//     }
//     ~ThrowOnN() {}
// };
// int ThrowOnN::count = 0;

// void test_vector_resize_exception_safety() {
//     vector<ThrowOnN> v;
//     try {
//         v.resize(10);  // 应在构造5个对象时中断，析构前4个
//     } catch (...) {
//         assert(ThrowOnN::count == 5);
//         // assert析构次数 == 4
//     }
// }

// void test_vector_basic() {
//     vector<std::string> v;
//     v.push_back("a");
//     v.push_back("b");
//     v.push_back("c");
//     assert(v.size() == 3);
//     assert(v[1] == "b");
//     v.resize(5);
//     assert(v.size() == 5);
//     v.resize(2);
//     assert(v.size() == 2);
//     std::cout << "[PASS] test_vector_basic\n";
// }

// struct Tracked {
//     static int copy_ctor, move_ctor;
//     Tracked() = default;
//     Tracked(const Tracked&) { ++copy_ctor; }
//     Tracked(Tracked&&) noexcept { ++move_ctor; }
// };
// int Tracked::copy_ctor = 0;
// int Tracked::move_ctor = 0;

// void test_vector_copy_move() {
//     vector<Tracked> v1;
//     v1.resize(3);

//     Tracked::copy_ctor = 0;
//     Tracked::move_ctor = 0;

//     vector<Tracked> v2 = v1;  // 拷贝构造
//     assert(Tracked::copy_ctor == 3);

//     Tracked::copy_ctor = 0;
//     Tracked::move_ctor = 0;

//     vector<Tracked> v3 = std::move(v1);  // 移动构造
//     assert(Tracked::move_ctor == 3);
// }

// void test_vector_iterator() {
//     vector<int> v;
//     v.push_back(1);
//     v.push_back(2);
//     v.push_back(3);

//     int sum = 0;
//     for (int x : v) sum += x;
//     assert(sum == 6);
// }

// void test_vector_empty_resize() {
//     vector<int> v;
//     v.resize(0);  // 不应崩溃
//     assert(v.size() == 0);
// }

// #define COMPARE_STD_NSTD(op) do {       \
//     std::vector<int> a = {1, 2, 3};     \
//     nstd::vector<int> b = {1, 2, 3};    \
//     a.op;                               \
//     b.op;                               \
//     assert(a == b);                     \
// } while(0)

// void test_compare_behavior() {
//     COMPARE_STD_NSTD(resize(5));
//     COMPARE_STD_NSTD(pop_back());
// }



void test_allocator_basic() {
    allocator<int> alloc;
    int* p = alloc.allocate(5);
    for (int i = 0; i < 5; ++i)
        alloc.construct(p + i, i);
    for (int i = 0; i < 5; ++i)
        assert(p[i] == i);
    for (int i = 0; i < 5; ++i)
        alloc.destroy(p + i);
    alloc.deallocate(p, 5);
    std::cout << "[PASS] test_allocator_basic\n";
}

void test_compressed_pair_ebo() {
    struct Empty {};
    compressed_pair<Empty, int> p;
    static_assert(sizeof(p) == sizeof(int), "EBO failed");
    std::cout << "[PASS] test_compressed_pair_ebo\n";
}

void test_compressed_pair_access() {
    compressed_pair<std::string, int> p("hello", 123);
    assert(p.first() == "hello");
    assert(p.second() == 123);
    std::cout << "[PASS] test_compressed_pair_access\n";
}

struct Tracker {
    static int ctor, dtor;
    Tracker() {
        ++ctor;
    }
    ~Tracker() {
        ++dtor;
    }
};
int Tracker::ctor = 0;
int Tracker::dtor = 0;

void test_vector_lifecycle() {
    Tracker::ctor = 0;
    Tracker::dtor = 0;

    {
        vector<Tracker> v;
        v.resize(10);
        v.resize(0);
    }
    assert(Tracker::ctor == 10);
    assert(Tracker::dtor == 10);
    std::cout << "[PASS] test_vector_lifecycle\n";
}

int main() {
    test_allocator_basic();
    test_compressed_pair_ebo();
    test_compressed_pair_access();
    // test_vector_basic();
    test_vector_lifecycle();
    std::cout << "All tests passed!\n";
    return 0;
}

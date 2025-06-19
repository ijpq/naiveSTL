// tests/test_vector.cpp
#include "allocator.h"
#include "core/compressed_pair.h"
#include "vector.h" // 假设你的实现位于 nstd 命名空间
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

// TODO
struct ThrowOnN {
    static int count;
    static int count2;
    ThrowOnN() {
        if (++count, ++count2 == 5)
            throw std::runtime_error("fail");
    }
    ~ThrowOnN() {}

    int value = 2;
};
int ThrowOnN::count  = 0;
int ThrowOnN::count2 = 0;

void test_vector_resize_exception_safety() {
    nstd::vector<ThrowOnN> nstdv(1);
    std::vector<ThrowOnN> stdv(1);
    try {
        nstdv.resize(10); // 应在构造5个对象时中断，析构前4个
        stdv.resize(10);  // 应在构造5个对象时中断，析构前4个
    } catch (...) {
        assert(ThrowOnN::count == 5);
        assert(ThrowOnN::count == 5);
        assert(nstdv.size() == 1);
        assert(stdv.size() == 1);
        // assert析构次数 == 4
    }
    std::cout << "[PASS] test_vector_resize_exception_safety\n";
}

void test_vector_basic() {
    nstd::vector<std::string> v;
    v.push_back("a");
    v.push_back("b");
    v.push_back("c");
    assert(v.size() == 3);
    assert(v[0] == "a");
    assert(v[1] == "b");
    assert(v[2] == "c");
    v.resize(5);
    assert(v.size() == 5);
    assert(v[3] == "");
    assert(v[4] == "");
    v.resize(2);
    assert(v.size() == 2);
    assert(v[0] == "a");
    assert(v[1] == "b");
    std::cout << "[PASS] test_vector_basic\n";
}

struct Tracked {
    static int copy_ctor, move_ctor;
    Tracked() = default;
    Tracked(const Tracked&) {
        ++copy_ctor;
    }
    Tracked(Tracked&&) noexcept {
        ++move_ctor;
    }
    Tracked& operator=(const Tracked& t) {
        ++copy_ctor;
        return *this;
    }

    Tracked& operator=(Tracked&& t) {
        ++move_ctor;
        return *this;
    }
};
int Tracked::copy_ctor = 0;
int Tracked::move_ctor = 0;

void test_vector_copy_move() {
    nstd::vector<Tracked> v1;
    v1.resize(3);

    Tracked::copy_ctor = 0;
    Tracked::move_ctor = 0;

    nstd::vector<Tracked> v2 = v1; // 拷贝构造
    assert(Tracked::copy_ctor == 3);

    nstd::vector<Tracked> v3(v1); // 拷贝构造
    assert(Tracked::copy_ctor == 6);

    Tracked::copy_ctor = 0;
    Tracked::move_ctor = 0;

    nstd::vector<Tracked> v4 = std::move(v1); // 移动构造
    assert(Tracked::move_ctor == 3);

    nstd::vector<Tracked> v5(std::move(v1)); // 移动构造
    assert(Tracked::move_ctor == 6);
    std::cout << "[PASS] test_vector_copy_move\n";
}

// void test_vector_iterator() {
//     nstd::vector<int> v;
//     v.push_back(1);
//     v.push_back(2);
//     v.push_back(3);

//     int sum = 0;
//     for (int x : v) sum += x;
//     assert(sum == 6);
// }

void test_vector_empty_resize() {
    nstd::vector<int> v;
    v.resize(0); // 不应崩溃
    assert(v.size() == 0);
    std::cout << "[PASS] test_vector_empty_resize\n";
}

#define COMPARE_STD_NSTD(op)             \
    do {                                 \
        std::vector<int> a  = {1, 2, 3}; \
        nstd::vector<int> b = {1, 2, 3}; \
        a.op;                            \
        b.op;                            \
        assert(a == b);                  \
    } while (0)

void test_compare_behavior() {
    COMPARE_STD_NSTD(resize(5));
    COMPARE_STD_NSTD(pop_back());
    std::cout << "[PASS] test_compare_behavior\n";
}

void test_allocator_basic() {
    nstd::allocator<int> alloc;
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
    nstd::compressed_pair<Empty, int> p;
    static_assert(sizeof(p) == sizeof(int), "EBO failed");
    std::cout << "[PASS] test_compressed_pair_ebo\n";
}

void test_compressed_pair_access() {
    nstd::compressed_pair<std::string, int> p("hello", 123);
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
        nstd::vector<Tracker> v;
        v.resize(10);
        v.resize(0);
    }
    assert(Tracker::ctor == 10);
    assert(Tracker::dtor == 10);
    std::cout << "[PASS] test_vector_lifecycle\n";
}

int main() {
    test_vector_resize_exception_safety();
    test_vector_empty_resize();
    test_allocator_basic();
    test_compressed_pair_ebo();
    test_compressed_pair_access();
    test_vector_basic();
    test_vector_lifecycle();
    test_vector_copy_move();
    test_compare_behavior();
    std::cout << "All tests passed!\n";
    return 0;
}

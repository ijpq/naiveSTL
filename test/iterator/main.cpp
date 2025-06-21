#include "core/config.h"
#include "iterator.h"
#include <cassert>
using namespace nstd;
template <typename T>
using RandomAccessIterator = Iterator<T>;
void test_random_access_iterator() {
    int arr[] = {10, 20, 30, 40, 50};

    RandomAccessIterator<int> it1(arr);     // 指向 arr[0]
    RandomAccessIterator<int> it2(arr + 3); // 指向 arr[3]

    // 解引用
    assert(*it1 == 10);
    assert(*it2 == 40);

    // 差值
    assert(it2 - it1 == 3);
    assert(it1 - it2 == -3);

    // 指针加法
    assert(*(it1 + 1) == 20);
    assert(*(1 + it1) == 20); // 若你支持这个（通过非成员 operator+）

    // 随机访问
    assert(it1[2] == 30);

    // 比较
    assert(it1 < it2);
    assert(it2 > it1);
    assert(it1 <= it2);
    assert(it1 <= it1);
    assert(it2 >= it1);

    // 自增自减
    auto tmp = it1;
    ++tmp;
    assert(*tmp == 20);
    tmp--;
    assert(*tmp == 10);

    std::cout << "All RandomAccessIterator tests passed.\n";
}

int main() {
    test_random_access_iterator();
}
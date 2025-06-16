#ifndef B6A07B06_B01A_42CD_8E70_F41ADD80B688
#define B6A07B06_B01A_42CD_8E70_F41ADD80B688

#include "core/config.h"
#include <cstddef>
#include <stdexcept>
#include <type_traits>

NAMESPACE_NAIVE_STD_BEGIN

template <typename T>
struct allocator {
    // allocator requirement:
    using pointer            = T*;
    using const_pointer      = const T*;
    using void_pointer       = void*;
    using const_void_pointer = const void*;
    using value_type         = T;

    // std
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;
    /*
    对于 std::allocator<T>：
    move assignment 时：allocator 被一起 move，是为了最大化资源接管性能，且
    std::allocator 是无状态的，不会出错； copy assignment 时：allocator 也被
    copy，目的是让新容器和原容器拥有一致的内存分配行为，避免状态不一致带来的隐患；
    因此两个 propagate traits 都设为 true_type 是合理的、安全的，并且符合 STL
    的一致性原则。
    */
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal =
        std::true_type; // for any object a1, a2 of `stateless` allocator,  a1
                        // == a2 should be guaranteed. allocator from STL is
                        // stateless allocator

    pointer allocate(size_type n) {
        if (n > max_size())
            throw std::bad_alloc();
        std::cout << "not constant evaluated" << std::endl;
        if (alignof(T) > alignof(std::max_align_t))
            return static_cast<pointer>(
                ::operator new(n * sizeof(T), std::align_val_t(alignof(T))));
        else
            return static_cast<pointer>(::operator new(n * sizeof(T)));
    }

    static size_type max_size() {
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }

    void deallocate(pointer p, size_type n) {
        return ::operator delete(p, alignof(T));
    }

    template <typename Tp, typename... Args>
    void construct(Tp* p, Args&&... args) {
        ::new (p) Tp(std::forward<Args>(args)...);
    }

    // move this func to traits after it's implemented
    template <typename Tp, bool = true, typename... Args>
    static void construct(Tp* p, Args&&... args) {
        ::new (p) Tp(std::forward<Args>(args)...);
        return;
    }

    template <typename Tp>
    void destroy(Tp* p) {
        p->~Tp();
    }

    template <typename U>
    struct rebind {
        using other = allocator<U>;
    };

    allocator select_on_container_copy_construction() {
        return *this;
    }

    template <typename Up>
    bool operator==(const allocator<Up>& rhs) {
        return true;
    }

    template <typename Up>
    bool operator!=(const allocator<Up>& rhs) {
        return false;
    }

    // void* operator new(size_type n) {
    //     return 0;
    // }
};

NAMESPACE_NAIVE_STD_END

#endif /* B6A07B06_B01A_42CD_8E70_F41ADD80B688 */

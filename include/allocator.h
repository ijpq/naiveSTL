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

    pointer allocate(size_type n) {
        if (n > max_size())
            throw std::bad_alloc();
        if (is_constant_evaluated()) {
            std::cout << "is constant evaluated" << std::endl;
            return static_cast<pointer>(::operator new(n * sizeof(T)));
        } else {
            std::cout << "not constant evaluated" << std::endl;
            if (alignof(T) > alignof(std::max_align_t))
                return static_cast<pointer>(::operator new(n * sizeof(T), std::align_val_t(alignof(T))));
            else
                return static_cast<pointer>(::operator new(n * sizeof(T)));
        }
    }

    size_type max_size() {
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }

    void deallocate(pointer p, size_type n) {
        if (is_constant_evaluated())
            return ::operator delete(p);
        else
            return ::operator delete(p, alignof(T));
    }

    template<typename Tp, typename ...Args>
    void construct(Tp* p, Args&&... args) {
        ::new(p) Tp(std::forward<Args>(args)...);
    }

    template<typename Tp>
    void destroy(Tp* p) {
        p->~Tp();
    }
};

NAMESPACE_NAIVE_STD_END

#endif /* B6A07B06_B01A_42CD_8E70_F41ADD80B688 */

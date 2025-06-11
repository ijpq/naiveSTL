#ifndef B6A07B06_B01A_42CD_8E70_F41ADD80B688
#define B6A07B06_B01A_42CD_8E70_F41ADD80B688

#include "core/config.h"
#include <cstddef>

NAMESPACE_NAIVE_STD_BEGIN

template<typename T>
struct allocator {
    // allocator requirement: 
    using pointer = T*;
    using const_pointer = const T*;
    using void_pointer = void*;
    using const_void_pointer = const void*;
    using value_type = T;

    // std
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    
    // todo
    // T& operator*() {}
    
    pointer allocate(size_type n) {
        if (is_constant_evaluated()) {
            return static_cast<pointer>(::operator new(n * sizeof(T))); 
        } else {
            return static_cast<pointer>(::operator new(n*sizeof(T), )
        }
    }
};

NAMESPACE_NAIVE_STD_END

#endif /* B6A07B06_B01A_42CD_8E70_F41ADD80B688 */

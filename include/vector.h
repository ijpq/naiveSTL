#include "allocator.h"
#include "core/compressed_pair.h"
#include "core/config.h"
#include <stdexcept>

NAMESPACE_NAIVE_STD_BEGIN
template <typename T, typename _Allocator = allocator<T>>
struct vector {
    using default_allocator_type = allocator<T>;
    using value_type             = T;
    using allocator_type         = _Allocator;
    // todo
    using __alloc_traits = _Allocator;

    using reference       = value_type&;
    using const_reference = const value_type&;
    using size_type       = typename __alloc_traits::size_type;
    using difference_type = typename __alloc_traits::difference_type;
    using pointer         = typename __alloc_traits::pointer;
    using const_pointer   = typename __alloc_traits::const_pointer;

private:
    pointer                                  _begin = nullptr;
    pointer                                  _end   = nullptr;
    compressed_pair<pointer, allocator_type> _end_cap{nullptr,
                                                      _default_init_tag()};

public:
    // todo: iterator
    static_assert(
        std::is_same_v<typename __alloc_traits::value_type, value_type>);

    vector() {}

    explicit vector(size_type n);

    explicit vector(size_type n, const allocator_type& a);

    vector(size_type n, const value_type& x);

    template <
        typename = std::enable_if_t<nstd::is_allocator<allocator_type>::value>>
    vector(size_type n, const value_type& x, const allocator_type& a) :
    _end_cap(nullptr, a) {
        if (n > 0) {
            _vallocate(n);
            _construct_at_end(n, x);
        }
    }

    allocator_type& __alloc() {
        return this->_end_cap.second();
    }

    const allocator_type& __alloc() const {
        return this->_end_cap.second();
    }

    void _vallocate(size_type n) {
        if (n > max_size())
            throw std::length_error("");
        typename __alloc_traits::pointer p = _end_cap.second().allocate(n);
        _begin                             = p;     // begin pos of valid elems
        _end                               = p;     // end pos of valid elems
        _end_cap.first()                   = p + n; // end pos of capacity
#ifdef ENABLE_ASAN
// todo
#endif
    }

    struct ConstructTransaction {
        // denote the
        vector&       _vec;
        pointer       _pos; // end pos of valid elems
        const_pointer _end; // end pos of capacity
        explicit ConstructTransaction(const vector& vec, size_type n) :
        _vec(vec),
        _pos(vec._end),
        _end(vec._end + n) {}

        ~ConstructTransaction() {
            _vec._end = _pos;
        }
    };

    void _construct_at_end(size_type n, value_type x) {
        ConstructTransaction tx{*this, n};
        for (pointer _pos = tx._pos; _pos != tx._end; tx._pos = ++_pos)
            __alloc_traits::construct(_pos, x);
    }

    size_type max_size() {
        return __alloc_traits::max_size();
    }
};

NAMESPACE_NAIVE_STD_END
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
    pointer _begin = nullptr;
    pointer _end   = nullptr;
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

    ~vector() {
        clear();
        this->__alloc().deallocate(this->_begin, capacity());
    }

    size_type size() {
        return static_cast<size_type>(_end - _begin);
    }

    size_type capacity() {
        return static_cast<size_type>(_end_cap.first() - _begin);
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

    // pos tag of valid elems is updated by this, during construct
    struct ConstructTransaction {
        // denote the
        vector& _vec;
        pointer _pos;       // end pos of valid elems
        const_pointer _end; // end pos of capacity
        explicit ConstructTransaction(vector& vec, size_type n) :
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
            // __alloc_traits::construct(_pos, x);
            this->__alloc().construct(_pos, x);
    }

    size_type max_size() {
        return __alloc_traits::max_size();
    }

    void _deconstruct_at_end(pointer new_last) {
        pointer last = this->_end;
        while (last != new_last) {
            this->__alloc().destroy(--last);
        }
        this->_end = last;
    }

    void clear() {
        _deconstruct_at_end(this->_begin);
    }

    void resize(size_type count) {
        if (count == size())
            return;
        if (size() > count) {
            _deconstruct_at_end(this->_begin + count);
        } else if (size() < count) {
            if (capacity() >= count) {
                _construct_at_end(count - size(), T());
            } else {
                pointer origin_p       = this->_begin;
                pointer origin_end     = this->_end;
                size_type old_capacity = capacity();
                _vallocate(count);
                for (pointer op = origin_p; op != origin_end;
                     op++, this->_end++) {
                    // *(this->_end) = std::move(*op);
                    this->__alloc().construct(this->_end, std::move(*op));
                    this->__alloc().destroy(op);
                }
                this->__alloc().deallocate(origin_p, old_capacity);
                _construct_at_end(count - size(), T());
            }
        }
    }

    void resize(size_type count, const value_type& value) {
        if (count == size())
            return;
        if (size() > count) {
            _deconstruct_at_end(this->_begin + count);
        } else if (size() < count) {
            if (capacity() >= count) {
                _construct_at_end(count - size(), value);
            } else {
                pointer origin_p       = this->_begin;
                pointer origin_end     = this->_end;
                size_type old_capacity = capacity();
                _vallocate(count);
                for (pointer op = origin_p; op != origin_end;
                     op++, this->_end++) {
                    // *(this->_end) = std::move(*op);
                    this->__alloc().construct(this->_end, std::move(*op));
                    this->__alloc().destroy(op);
                }
                this->__alloc().deallocate(origin_p, old_capacity);
                _construct_at_end(count - size(), value);
            }
        }
    }
};

NAMESPACE_NAIVE_STD_END
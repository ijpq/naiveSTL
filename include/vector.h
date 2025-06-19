#include "allocator.h"
#include "core/compressed_pair.h"
#include "core/config.h"
#include <initializer_list>
#include <stdexcept>
#include <type_traits>

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

    explicit vector(size_type n) : _end_cap(nullptr, _default_init_tag()) {
        auto rollback = registerRollback(destroy_vector(*this));
        if (n > 0) {
            _vallocate(n);
            _construct_at_end(n, T());
        }
        rollback.complete();
    }

    explicit vector(size_type n, const allocator_type& a);

    vector(size_type n, const value_type& x) {
        auto rollback = registerRollback(destroy_vector(*this));
        if (n > 0) {
            _vallocate(n);
            _construct_at_end(n, x);
        }
        rollback.complete();
    }

    template <
        typename = std::enable_if_t<nstd::is_allocator<allocator_type>::value>>
    vector(size_type n, const value_type& x, const allocator_type& a) :
    _end_cap(nullptr, a) {
        auto rollback = registerRollback(destroy_vector(*this));
        if (n > 0) {
            _vallocate(n);
            _construct_at_end(n, x);
        }
        rollback.complete();
    }

    vector(std::initializer_list<value_type> l) :
    _end_cap(nullptr, _default_init_tag()) {
        auto rollback  = registerRollback(destroy_vector(*this));
        size_type size = l.size();
        _vallocate(size);
        for (const value_type& v : l) {
            _construct_at_end(1, v);
        }

        rollback.complete();
    }

    vector(const vector& other) : _end_cap(nullptr, other.__alloc()) {
        auto rollback = registerRollback(destroy_vector(*this));
        _vallocate(other.capacity());
        std::copy_n(other._begin, other.size(), this->_begin);
        rollback.complete();
    }

    vector(vector&& other) :
    _end_cap(other.end_cap(), std::move(other.__alloc())) {
        this->_begin = other._begin;
        this->_end   = other._end;
        other._begin = other._end = nullptr;
    }

    ~vector() {
        destroy_vector (*this)();
    }

    pointer end_cap() {
        return this->_end_cap.first();
    }

    class destroy_vector {
        vector& vec;

    public:
        destroy_vector(vector& _vec) : vec(_vec) {}
        void operator()() {
            vec.clear();
            vec.__alloc().deallocate(vec._begin, vec.capacity());
        }
    };

    size_type size() {
        return static_cast<size_type>(_end - _begin);
    }

    size_type size() const {
        return static_cast<size_type>(_end - _begin);
    }

    size_type capacity() {
        return static_cast<size_type>(_end_cap.first() - _begin);
    }

    size_type capacity() const {
        return static_cast<size_type>(_end_cap.first() - _begin);
    }

    allocator_type& __alloc() {
        return this->_end_cap.second();
    }

    const allocator_type& __alloc() const {
        return this->_end_cap.second();
    }

    void _vallocate(const size_type& n) {
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

    void _shrink_to(size_type count) {
        _deconstruct_at_end(this->_begin + count);
    }

    // move this._end to new_last
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

    void _reallocate_and_expand_to(size_type count, value_type value) {
        pointer origin_p       = this->_begin;
        pointer origin_end     = this->_end;
        size_type old_capacity = capacity();

        auto rollback = registerRollback([&] {
            // 1. destroy 已构造的新对象
            for (pointer p = this->_begin; p != this->_end; ++p)
                this->__alloc().destroy(p);

            // 2. 将已 move 的对象移回原来的位置（必须记录范围）
            pointer new_p = this->_begin;
            pointer old_p = origin_p;
            for (; new_p != this->_end; ++new_p, ++old_p) {
                this->__alloc().construct(old_p, std::move(*new_p));
                this->__alloc().destroy(new_p); // 避免重复析构
            }

            // 3. 恢复指针状态
            this->__alloc().deallocate(this->_begin, count);
            this->_begin           = origin_p;
            this->_end             = origin_end;
            this->_end_cap.first() = origin_p + old_capacity;
        });

        _vallocate(count);

        for (pointer op = origin_p; op != origin_end; op++, this->_end++) {
            this->__alloc().construct(this->_end, std::move(*op));
        }

        _construct_at_end(count - size(), value);
        rollback.complete();
        for (pointer op = origin_p; op != origin_end; op++) {
            this->__alloc().destroy(op);
        }
        this->__alloc().deallocate(origin_p, old_capacity);
    }

    void _reallocate_and_expand_to(size_type count) {
        pointer origin_p       = this->_begin;
        pointer origin_end     = this->_end;
        size_type old_capacity = capacity();

        auto rollback = registerRollback([&] {
            // 1. destroy 已构造的新对象
            for (pointer p = this->_begin; p != this->_end; ++p)
                this->__alloc().destroy(p);

            // 2. 将已 move 的对象移回原来的位置（必须记录范围）
            pointer new_p = this->_begin;
            pointer old_p = origin_p;
            for (; new_p != this->_end; ++new_p, ++old_p) {
                this->__alloc().construct(old_p, std::move(*new_p));
                this->__alloc().destroy(new_p); // 避免重复析构
            }

            // 3. 恢复指针状态
            this->__alloc().deallocate(this->_begin, count);
            this->_begin           = origin_p;
            this->_end             = origin_end;
            this->_end_cap.first() = origin_p + old_capacity;
        });

        _vallocate(count);

        for (pointer op = origin_p; op != origin_end; op++, this->_end++) {
            this->__alloc().construct(this->_end, std::move(*op));
        }

        _construct_at_end(count - size(), T());
        rollback.complete();
        for (pointer op = origin_p; op != origin_end; op++) {
            this->__alloc().destroy(op);
        }
        this->__alloc().deallocate(origin_p, old_capacity);
    }

    // strong exception safety guarantee
    void resize(size_type count) {
        if (count == size())
            return;
        if (size() > count) {
            _shrink_to(count);
        } else if (size() < count) {
            if (capacity() >= count) {
                _construct_at_end(count - size(), T());
            } else {
                _reallocate_and_expand_to(count);
            }
        }
    }

    template <typename U>
    void _construct_one_at_end(U&& u) {
        ConstructTransaction tx(*this, 1);
        this->__alloc().construct(tx._pos, std::forward<U>(u));
        tx._pos++;
    }

    // strong exception safety guarantee
    template <
        typename U,
        std::enable_if_t<std::is_constructible_v<value_type, U&&>, int> = 0>
    void push_back(U&& u) {
        if (this->_end < this->_end_cap.first()) {
            _construct_one_at_end(std::forward<U>(u));
        } else {
            _reallocate_and_expand_to(size() + 1, std::forward<U>(u));
        }
    }

    void resize(size_type count, const value_type& value) {
        if (count == size())
            return;
        if (size() > count) {
            _shrink_to(count);
        } else if (size() < count) {
            if (capacity() >= count) {
                _construct_at_end(count - size(), value);
            } else {
                _reallocate_and_expand_to(count, value);
            }
        }
    }

    bool empty() {
        return this->_end == this->_begin;
    }

    bool empty() const {
        return this->_end == this->_begin;
    }

    void pop_back() {
        if (empty())
            return;
        _deconstruct_at_end(this->_end - 1);
    }

    value_type& operator[](size_type index) {
        return *(this->_begin + index);
    }
};

NAMESPACE_NAIVE_STD_END
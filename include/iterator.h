#include "core/config.h"
#include <cstddef>

struct LegacyIterator {};
struct LegacyRandomAccessIterator {};
struct LegacyForwardIterator {};
NAMESPACE_NAIVE_STD_BEGIN
template <typename Iter>
class Iterator {
public:
    template <typename _Tp>
    using iterator_traits = std::iterator_traits<_Tp>;

    using pointer           = Iter*;
    using value_type        = Iter;
    using difference_type   = std::ptrdiff_t;
    using reference         = Iter&;
    using const_reference   = const Iter&;
    using iterator_category = LegacyRandomAccessIterator;

    static_assert(std::is_same_v<reference, Iter&>);
    static_assert(!std::is_pointer_v<Iter>);

    Iterator(pointer p) : i(p) {}

    bool operator==(const Iterator& other) const {
        return this->i == other.i;
    }

    bool operator!=(const Iterator& other) const {
        return !(*this == other);
    }

    reference operator*() {
        return *this->i;
    }

    const_reference operator*() const {
        return *this->i;
    }

    Iterator& operator++() { // ++r
        ++i;
        return *this;
    }

    const Iterator& operator++() const { // ++r
        ++i;
        return *this;
    }
    Iterator operator++(int) {
        Iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    const Iterator operator++(int) const {
        Iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    Iterator& operator--() { // --r
        --i;
        return *this;
    }

    const Iterator& operator--() const { // --r
        --i;
        return *this;
    }

    const Iterator operator--(int) const {
        Iterator tmp = *this;
        --(*this);
        return tmp;
    }

    Iterator operator--(int) {
        Iterator tmp = *this;
        --(*this);
        return tmp;
    }

    Iterator& operator+=(difference_type n) {
        // if (n >= 0) while (n--) ++(*this);
        // else while (n++) --(*this);
        // return *this;
        this->i = this->i + n;
        return *this;
    }

    const Iterator& operator+=(difference_type n) const {
        // if (n >= 0) while (n--) ++(*this);
        // else while (n++) --(*this);
        // return *this;
        this->i = this->i + n;
        return *this;
    }

    Iterator operator+(difference_type n) {
        Iterator temp = *this;
        return temp += n;
    }

    const Iterator operator+(difference_type n) const {
        Iterator temp = *this;
        return temp += n;
    }

    Iterator& operator-=(difference_type n) {
        return (*this) += -n;
    }

    const Iterator& operator-=(difference_type n) const {
        return (*this) += -n;
    }

    difference_type operator-(const Iterator& b) {
        return this->i - b.i;
    }

    difference_type operator-(const Iterator& b) const {
        return this->i - b.i;
    }

    reference operator[](difference_type n) {
        return *(*this + n);
    }

    const_reference operator[](difference_type n) const {
        return *(*this + n);
    }

    bool operator<(const Iterator& b) const {
        return this->i < b.i;
    }

    bool operator>(const Iterator& b) const {
        return b < *this;
    }

    bool operator>=(const Iterator& b) const {
        return !(*this < b);
    }

    bool operator<=(const Iterator& b) const {
        return !(*this > b);
    }

private:
    Iter* i;
};

NAMESPACE_NAIVE_STD_END
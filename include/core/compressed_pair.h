#pragma once
#include "core/config.h"
#include <type_traits>
NAMESPACE_NAIVE_STD_BEGIN
template <typename T1, int index,
          bool isEmptyBase = std::is_empty_v<T1> && !std::is_final_v<T1>>
struct compressed_pair_elem {
    using value_type      = T1;
    using reference       = T1&;
    using const_reference = const T1&;
    compressed_pair_elem() : _value() {}

    template <typename U,
              std::enable_if_t<
                  (std::is_same_v<std::decay_t<U>, _default_init_tag> ||
                   std::is_same_v<std::decay_t<U>, _default_value_tag>) &&
                      std::is_default_constructible_v<T1>,
                  long> = 0>
    explicit compressed_pair_elem(U&& _) : _value() {}

    template <typename U,
              std::enable_if_t<
                  !std::is_same_v<std::decay_t<U>, _default_init_tag> &&
                      !std::is_same_v<std::decay_t<U>, _default_value_tag> &&
                      std::is_constructible_v<T1, U&&>,
                  int> = 0>
    compressed_pair_elem(U&& _u) : _value(_u) {}

    explicit compressed_pair_elem(const T1& _a) : _value(_a) {}
    explicit compressed_pair_elem(T1&& _a) : _value(std::forward<T1>(_a)) {}
    reference get() {
        return _value;
    }
    const_reference get() const {
        return _value;
    }

private:
    T1 _value;
};

template <typename T1, int index>
struct compressed_pair_elem<T1, index, true> : private T1 {
    using Base            = T1;
    using reference       = T1&;
    using const_reference = const T1&;

    template <std::enable_if_t<
                  std::is_default_constructible_v<std::decay_t<Base>>, int> = 0>
    compressed_pair_elem() : Base() {}

    template <
        typename U,
        std::enable_if_t<(std::is_same_v<std::decay_t<U>, _default_init_tag> ||
                          std::is_same_v<std::decay_t<U>, _default_value_tag>),
                         int> = 0>
    explicit compressed_pair_elem(U&& _u) : Base() {}

    reference get() {
        return static_cast<reference>(*this);
    }
    const_reference get() const {
        return static_cast<const_reference>(*this);
    }
};

template <typename T1, typename T2>
struct compressed_pair : private compressed_pair_elem<T1, 0>,
                         compressed_pair_elem<T2, 1> {
    using Base1 = compressed_pair_elem<T1, 0>;
    using Base2 = compressed_pair_elem<T2, 1>;

    compressed_pair() : Base1(), Base2() {}

    template <class _U1, class _U2>
    explicit compressed_pair(_U1&& __t1, _U2&& __t2) :
    Base1(std::forward<_U1>(__t1)),
    Base2(std::forward<_U2>(__t2)) {}

    typename Base1::reference first() {
        return static_cast<Base1&>(*this).get();
    }
    typename Base2::reference second() {
        return static_cast<Base2&>(*this).get();
    }
};
NAMESPACE_NAIVE_STD_END
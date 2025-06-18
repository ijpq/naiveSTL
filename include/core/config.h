#ifndef C4045081_0B58_4947_A97A_3F02474E6069
#define C4045081_0B58_4947_A97A_3F02474E6069
// clang-format off
#define NAMESPACE_NAIVE_STD_BEGIN namespace nstd {
#define NAMESPACE_NAIVE_STD_END }

#include <cstdio>
#include <cstddef>
#include <iostream>
#include <limits>
#include <cstddef>
#include <new>
#include <stdexcept>
#include <type_traits>

NAMESPACE_NAIVE_STD_BEGIN
inline constexpr bool is_constant_evaluated() {
#if defined(__clang__) || defined(__GNUG__)
    return __builtin_is_constant_evaluated();
#else
    return false;
#endif
}

template<typename T, typename = void>
struct is_allocator : std::false_type {};

template<typename T>
struct is_allocator<T, std::void_t<decltype(std::declval<T>().allocate(0))>> : std::true_type {};

struct _default_init_tag {};
struct _default_value_tag {};

template<typename Rollback>
struct RAIIRollback {
    
    RAIIRollback(Rollback _rollback) : rollback(_rollback) {}

    ~RAIIRollback() {rollback();}
    Rollback rollback;
};

template<typename T>
RAIIRollback<T> registerRollback(T func) {
    return RAIIRollback(func);
}
NAMESPACE_NAIVE_STD_END
#endif /* C4045081_0B58_4947_A97A_3F02474E6069 */

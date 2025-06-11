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


inline bool is_constant_evaluated() {
// clang and GNU seem the same built-in
#ifdef __clang__
return __builtin_is_constant_evaluated()
#else
#ifdef __GNU__
return __builtin_is_constant_evaluated()
#endif
#endif
;
}

#endif /* C4045081_0B58_4947_A97A_3F02474E6069 */

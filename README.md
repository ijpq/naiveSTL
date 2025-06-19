# naiveSTL
![CI](https://github.com/ijpq/naiveSTL/actions/workflows/test.yaml/badge.svg)

## Why NaiveSTL?

This project reimplements core components of the C++ STL from scratch. Unlike the official libcxx or libstdc++, **NaiveSTL** focuses on:

- ✨ Readability: Minimal template metaprogramming, clear structure.
- 🔍 Educational value: Aimed at developers who want to understand STL internals.
- ⚙️ Modern C++: Based on C++17 to reflect recent practices.
- 🧪 Test-driven: With unit tests and exception safety validations.
- 🎯 motivation: Since many teams are still using C++11/14 in industrial programming, and need a lightweight STL in realistic development.

## Features

- 📦 Containers: `vector`, 🚧: `constexpr vector`.
- 🧵 Allocator support: Custom allocator system.
- 🛠️ Exception safety: RAII + rollback mechanisms tested under `ThrowOnN` model.
- 🚦 CI-integrated: GitHub Actions + ASan to validate correctness and memory safety.
- 🔍 STL compatibility: Interfaces/Exception guarantee closely match standard STL.



## Acknowledgements

Inspired by:

- libcxx source code (LLVM)
- https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2024/p3372r0.htmlcpp17
- Working Draft, Standard for Programming Language C++Modern C++ STL design articles(N4659)

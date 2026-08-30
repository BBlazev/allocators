# allocators
 
A custom memory allocator library written in C++23. Built from scratch as standalone, dependency-free components with benchmarks and tests.
 
The goal is to understand how memory management actually works at a level below `new` and `delete`


## Benchmarks

10K allocations, 32-byte struct, AMD Ryzen, `-O3` + LTO.

| Method | Time | Speedup |
|---|---|---|
| Stack (baseline) | 1,084 ns | – |
| Linear | 4,429 ns | 15x vs new |
| Pool | 44,864 ns | ~6.4x vs new |
| new/delete | 68,362 ns | – |
| new/delete (churn) | 288,972 ns | – |





## Building
 
Requires CMake 3.25+, a C++23 compiler (GCC 13+ or Clang 17+), and Ninja.
 
```bash
# Debug 
cmake --preset=debug
cmake --build build/debug
 
# Release
cmake --preset=release
cmake --build build/release
 

```
 
## Running
 
```bash
# Tests
./build/debug/test_linear
 
# Benchmarks 
./build/release/benchmarks/bench_all
```
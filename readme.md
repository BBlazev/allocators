# allocators
 
A custom memory allocator library written in C++23. Built from scratch as standalone, dependency-free components with benchmarks and tests.
 
The goal is to understand how memory management actually works at a level below `new` and `delete`


## Benchmarks

### Linear Allocator
Allocator          | 10K allocs  | vs new/delete
------------------ | ----------- | -------------
Linear (arena)     |   4,394 ns  | 19x faster
Pool               |         ns  |
Ring               |         ns  |
Buddy              |         ns  |
Free list          |         ns  |
new/delete         |  83,743 ns  | baseline
Stack (reference)  |   1,089 ns  | theoretical max

2026-08-30T14:16:21+02:00
CPU Caches:
  L1 Data 32 KiB (x8)
  L1 Instruction 32 KiB (x8)
  L2 Unified 512 KiB (x8)
  L3 Unified 32768 KiB (x1)
Load Average: 1.43, 1.40, 1.28
-------------------------------------------------------------
Benchmark                   Time             CPU   Iterations
-------------------------------------------------------------
BM_GlobalAllocator      84042 ns        83743 ns         9813
BM_ArenaAllocator        4408 ns         4394 ns       159192
BM_StackAllocation       1093 ns         1089 ns       643842



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
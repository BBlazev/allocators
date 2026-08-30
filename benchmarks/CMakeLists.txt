include(FetchContent)

FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark
    GIT_TAG        v1.9.1
    GIT_SHALLOW    ON
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(benchmark)

target_compile_options(benchmark PRIVATE -w)

add_executable(bench_all main.cpp)
target_link_libraries(bench_all PRIVATE allocators benchmark::benchmark_main)
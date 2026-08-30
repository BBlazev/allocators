#include <benchmark/benchmark.h>
#include <allocators/LinearAllocator.hpp>

struct Entity {
    uint64_t data[4];
};

static void BM_GlobalAllocator(benchmark::State& state) {
    for (auto _ : state) {
        for (int i = 0; i < 10000; ++i) {
            Entity* e = new Entity();
            benchmark::DoNotOptimize(e);
            delete e;
        }
    }
}
BENCHMARK(BM_GlobalAllocator);

static void BM_ArenaAllocator(benchmark::State& state) {
    LinearAllocator arena(10000 * sizeof(Entity));

    for (auto _ : state) {
        for (int i = 0; i < 10000; ++i) {
            void* mem = arena.Allocate(sizeof(Entity), alignof(Entity));
            Entity* e = new (mem) Entity();
            benchmark::DoNotOptimize(e);
        }
        arena.Reset();
    }
}
BENCHMARK(BM_ArenaAllocator);

static void BM_StackAllocation(benchmark::State& state) {
    for (auto _ : state) {
        for (int i = 0; i < 10000; ++i) {
            Entity e;
            benchmark::DoNotOptimize(e);
        }
    }
}
BENCHMARK(BM_StackAllocation);

static void BM_ArenaAllocatorTemplate(benchmark::State& state){
    LinearAllocator arena(10000 * sizeof(Entity));
    for(auto _ : state){
        for(int i = 0; i < 10000; ++i){
            Entity* e = arena.Allocate<Entity>();
            benchmark::DoNotOptimize(e);
        }
        arena.Reset();
    }
}
BENCHMARK(BM_ArenaAllocatorTemplate);



BENCHMARK_MAIN();
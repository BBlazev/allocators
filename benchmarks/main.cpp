#include <benchmark/benchmark.h>
#include <allocators/LinearAllocator.hpp>
#include <allocators/PoolAllocator.hpp>

#include <algorithm>
#include <random>
#include <vector>
#include <ranges>


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

std::vector<int> GetRandomIndices(int count) {
  std::vector<int> indices(count);
  for (int i = 0; i < count; ++i) indices[i] = i;
  std::mt19937 g(42);
  std::ranges::shuffle(indices, g);
  return indices;
}

static void BM_GlobalHeap_Churn(benchmark::State& state) {
  std::vector<int> destroyOrder = GetRandomIndices(10000);
  std::vector<Entity*> active(10000);

  for (auto _ : state) {
    for (int i = 0; i < 10000; ++i) {
      active[i] = new Entity();
    }
    for (int idx : destroyOrder) {
      delete active[idx];
    }
  }
}
BENCHMARK(BM_GlobalHeap_Churn);

static void BM_ObjectPool_Churn(benchmark::State& state) {
  std::vector<int> destroyOrder = GetRandomIndices(10000);
  std::vector<Entity*> active(10000);

  PoolAllocator<Entity> MyPool(10000);

  for (auto _ : state) {
    for (int i = 0; i < 10000; ++i) {
      active[i] = MyPool.Allocate();
    }
    for (int idx : destroyOrder) {
      MyPool.Deallocate(active[idx]);
    }
  }
}
BENCHMARK(BM_ObjectPool_Churn);

BENCHMARK_MAIN();
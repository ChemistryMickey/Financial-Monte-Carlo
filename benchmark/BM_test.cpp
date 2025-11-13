#include "benchmark/benchmark.h"

#define N_SHOTS 1000

struct Foo
{
    double a[100] = {};
};

Foo getafoo()
{
    return Foo{};
}

void bind_by_const_ref()
{
    for (int i = 0; i < N_SHOTS; ++i)
    {
        const Foo& foo = getafoo();
        benchmark::DoNotOptimize(foo);
    }
}

void bind_by_value()
{
    for (int i = 0; i < N_SHOTS; ++i)
    {
        const Foo foo = getafoo();
        benchmark::DoNotOptimize(foo);
    }
}

#define BM_TEST(func, ...)                         \
    static void BM_##func(benchmark::State &state) \
    {                                              \
        for (auto _ : state)                       \
            func(__VA_ARGS__);                     \
    }                                              \
    BENCHMARK(BM_##func);

BM_TEST(bind_by_value);
BM_TEST(bind_by_const_ref);

BENCHMARK_MAIN();

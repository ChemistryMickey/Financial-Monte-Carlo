#include "benchmark/benchmark.h"
#include <chrono>
#include <random>
using namespace std::chrono;

#define N_SHOTS 1000000

/*
Result:
----------------------------------------------------------------------
Benchmark                            Time             CPU   Iterations
----------------------------------------------------------------------
BM_increment_integer          10024686 ns     10021015 ns           69
BM_increment_sys_days         10037776 ns     10036103 ns           70
BM_increment_year_month_day    9950934 ns      9949496 ns           70

This is a wash. The `rand()` is much more expensive than the actual increment.
*/

void increment_integer() {
    int val = 0;
    benchmark::DoNotOptimize(val);
    for (int i = 0; i < N_SHOTS; ++i) {
        val += rand() % 10;
    }
}

void increment_sys_days() {
    sys_days d = 2020y / 1 / 1;
    benchmark::DoNotOptimize(d);
    for (int i = 0; i < N_SHOTS; ++i) {
        d += days{rand() % 10};
    }
}

void increment_year_month_day() {
    year_month_day d = 2020y / 1 / 1;
    benchmark::DoNotOptimize(d);
    for (int i = 0; i < N_SHOTS; ++i) {
        d = year_month_day{sys_days{d} + days{rand() % 10}};
    }
}

#define BM_TEST(func, ...)                         \
    static void BM_##func(benchmark::State &state) \
    {                                              \
        for (auto _ : state)                       \
            func(__VA_ARGS__);                     \
    }                                              \
    BENCHMARK(BM_##func);

BM_TEST(increment_integer);
BM_TEST(increment_sys_days);
BM_TEST(increment_year_month_day);

BENCHMARK_MAIN();

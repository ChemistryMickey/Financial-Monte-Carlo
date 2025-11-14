#include "benchmark/benchmark.h"
#include "MonteCarloVariable.hpp"

#define N_SHOTS 1000
/*
Result (-O3):
----------------------------------------------------------------
Benchmark                      Time             CPU   Iterations
----------------------------------------------------------------
BM_sample_normal_more      62766 ns        62760 ns        10815
BM_by_icdf                 42677 ns        42663 ns        16437

The ICDF method is significantly faster
*/

void sample_normal_more() {
    static fmc::RandomVariable mc_var{0, fmc::GaussianDistributionArgs{.mean = 0, .stddev = 1}, 42};
    for (int i = 0; i < N_SHOTS; ++i) {
        while (true) {
            double val = mc_var.next_value();
            benchmark::DoNotOptimize(val);
            if (val > -1 && val < 1) {
                break;
            }
        }
    }

}

void by_icdf() {
    static fmc::RandomVariable mc_var{0, fmc::TruncatedGaussianDistributionArgs{.limits{.lower = -1, .upper = 1}, .normal{.mean = 0, .stddev = 1}}, 42};
    for (int i = 0; i < N_SHOTS; ++i) {
        double val = mc_var.next_value();
        benchmark::DoNotOptimize(val);
    }
}



#define BM_TEST(func, ...)                         \
    static void BM_##func(benchmark::State &state) \
    {                                              \
        for (auto _ : state)                       \
            func(__VA_ARGS__);                     \
    }                                              \
    BENCHMARK(BM_##func);

BM_TEST(sample_normal_more);
BM_TEST(by_icdf);

BENCHMARK_MAIN();

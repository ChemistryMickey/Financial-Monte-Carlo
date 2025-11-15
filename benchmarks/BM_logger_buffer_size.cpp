#include "benchmark/benchmark.h"
#include "include/Logger.hpp"

#define N_LINES 10000

/*
Result:
---------------------------------------------------------------
Benchmark                     Time             CPU   Iterations
---------------------------------------------------------------
BM_logger_buffer/1    108319487 ns    108263681 ns            7
BM_logger_buffer/8     47436905 ns     47391974 ns           15
BM_logger_buffer/64    37630282 ns     37594320 ns           19
BM_logger_buffer/500   36006250 ns     35980164 ns           19

Diminishing returns. So set this default to be 100 or something.
*/

void logger_buffer(fmc::Logger& logger) {
    for (int i = 0; i < N_LINES; ++i) {
        INFO("There once was a man from Peru, who dreamed he was eating his shoe. He woke with a fright in the middle of the night to find that his dream had come true!");
    }
}

static void BM_logger_buffer(benchmark::State& state) {
    fmc::Logger logger{"benchmark_test.log", state.range(0)};

    for (auto _ : state) {
        logger_buffer(logger);
    }
}

BENCHMARK(BM_logger_buffer)->Range(1, 500);

BENCHMARK_MAIN();

#include "benchmark/benchmark.h"
#include "include/Logger.hpp"

#define N_LINES 10000

/*
Result:
----------------------------------------------------------------
Benchmark                      Time             CPU   Iterations
----------------------------------------------------------------
BM_logger_buffer/1     108386755 ns    108119503 ns            7
BM_logger_buffer/8      47418229 ns     47280178 ns           15
BM_logger_buffer/64     37641651 ns     37574252 ns           19
BM_logger_buffer/512    36084552 ns     36003703 ns           19
BM_logger_buffer/4096   36641108 ns     36590624 ns           19
BM_logger_buffer/5000   36044071 ns     36008544 ns           19

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

BENCHMARK(BM_logger_buffer)->Range(1, 5000);

BENCHMARK_MAIN();

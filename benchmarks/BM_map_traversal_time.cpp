#include "benchmark/benchmark.h"
#include <map>
#include <unordered_map>
#include <string>
#include <random>

#define N_SHOTS 1000

void traverse_rb_tree_by_string() {
    static std::map<std::string, int> m{
        {"There once was a man from Peru", 0},
        {"Who dreamed he was eating his shoe", 1},
        {"He woke with a fright", 2},
        {"In the middle of the night", 3},
        {"To find that his dream had come true", 4}
    };

    for (auto& [k, v] : m) {
        int b = v;
        benchmark::DoNotOptimize(b);
    }
}

void traverse_rb_tree_by_int() {
    static std::map<int, std::string> m{
        {0, "There once was a man from Peru"},
        {1, "Who dreamed he was eating his shoe"},
        {2, "He woke with a fright"},
        {3, "In the middle of the night"},
        {4, "To find that his dream had come true"}
    };

    for (auto& [k, v] : m) {
        int b = k;
        benchmark::DoNotOptimize(b);
    }
}

void traverse_hash_map_by_string() {
    static std::unordered_map<std::string, int> m{
        {"There once was a man from Peru", 0},
        {"Who dreamed he was eating his shoe", 1},
        {"He woke with a fright", 2},
        {"In the middle of the night", 3},
        {"To find that his dream had come true", 4}
    };

    for (auto& [k, v] : m) {
        int b = v;
        benchmark::DoNotOptimize(b);
    }
}

void traverse_hash_map_by_int() {
    static std::unordered_map<int, std::string> m{
        {0, "There once was a man from Peru"},
        {1, "Who dreamed he was eating his shoe"},
        {2, "He woke with a fright"},
        {3, "In the middle of the night"},
        {4, "To find that his dream had come true"}
    };

    for (auto& [k, v] : m) {
        int b = k;
        benchmark::DoNotOptimize(b);
    }
}

#define BM_TEST(func, ...)                         \
    static void BM_##func(benchmark::State &state) \
    {                                              \
        for (auto _ : state)                       \
            func(__VA_ARGS__);                     \
    }                                              \
    BENCHMARK(BM_##func);

BM_TEST(traverse_rb_tree_by_string);
BM_TEST(traverse_hash_map_by_string);
BM_TEST(traverse_rb_tree_by_int);
BM_TEST(traverse_hash_map_by_int);

BENCHMARK_MAIN();

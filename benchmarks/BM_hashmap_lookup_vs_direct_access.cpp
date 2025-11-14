#include "benchmark/benchmark.h"
#include <unordered_map>
#include <map>

#define N_SHOTS 1000

struct Foo {
    int a = 42;
};

void access_via_direct_access() {
    static Foo foo{};

    for (int i = 0; i < N_SHOTS; ++i) {
        int a = foo.a;
        benchmark::DoNotOptimize(a);
    }
}

void access_via_unordered_map() {
    static std::unordered_map<const char*, int> foo{{"a", 42}};

    for (int i = 0; i < N_SHOTS; ++i) {
        int a = foo["a"];
        benchmark::DoNotOptimize(a);
    }
}

void access_via_map() {
    static std::map<const char*, int> foo{{"a", 42}};

    for (int i = 0; i < N_SHOTS; ++i) {
        int a = foo["a"];
        benchmark::DoNotOptimize(a);
    }
}

void assign_via_direct_access() {
    static Foo foo{};

    for (int i = 0; i < N_SHOTS; ++i) {
        foo.a = i;
    }
}

void assign_via_unordered_map() {
    static std::unordered_map<const char*, int> foo{{"a", 42}};

    for (int i = 0; i < N_SHOTS; ++i) {
        foo["a"] = i;
    }
}

void assign_via_map() {
    static std::map<const char*, int> foo{{"a", 42}};

    for (int i = 0; i < N_SHOTS; ++i) {
        foo["a"] = i;
    }
}

#define BM_TEST(func, ...)                         \
    static void BM_##func(benchmark::State &state) \
    {                                              \
        for (auto _ : state)                       \
            func(__VA_ARGS__);                     \
    }                                              \
    BENCHMARK(BM_##func);

BM_TEST(access_via_direct_access);
BM_TEST(access_via_unordered_map);
BM_TEST(access_via_map);
BM_TEST(assign_via_direct_access);
BM_TEST(assign_via_unordered_map);
BM_TEST(assign_via_map);

BENCHMARK_MAIN();

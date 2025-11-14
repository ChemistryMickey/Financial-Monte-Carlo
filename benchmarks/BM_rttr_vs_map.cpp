#include "benchmark/benchmark.h"
#include <map>
#include "rttr/registration"

/*
Result:
----------------------------------------------------------------------
Benchmark                            Time             CPU   Iterations
----------------------------------------------------------------------
BM_access_via_direct_access        120 ns          120 ns      5794788
BM_access_via_map                 1161 ns         1161 ns       603116
BM_access_via_rttr               41495 ns        41488 ns        17120
BM_access_via_rttr_caching       22235 ns        22233 ns        31404
BM_assign_via_direct_access       1.03 ns         1.03 ns    677333292
BM_assign_via_map                 1155 ns         1155 ns       603921
BM_assign_via_rttr               12863 ns        12859 ns        54589
BM_assign_via_rttr_caching        8380 ns         8380 ns        82677

Yikes, RTTR is so bad compared to a simple map that this is pretty insane.
Is there any way to make RTTR faster?

Caching improves performance quite a bit but not hugely so.
Luckily this isn't hot loop functionality; it only needs to happen once at the beginning of a sim.
*/

#define N_SHOTS 1000

struct Foo {
    int a = 42;
};

RTTR_REGISTRATION{
    rttr::registration::class_<Foo>("Foo")
        .property("a", &Foo::a);
}

void access_via_direct_access() {
    static Foo foo{};

    for (int i = 0; i < N_SHOTS; ++i) {
        int a = foo.a;
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

void access_via_rttr() {
    static Foo foo{};

    for (int i = 0; i < N_SHOTS; ++i) {
        int a = rttr::type::get(foo).get_property_value("a").to_int();
        benchmark::DoNotOptimize(a);
    }
}

void access_via_rttr_caching() {
    static Foo foo{};
    rttr::property prop_a = rttr::type::get(foo).get_property("a");

    for (int i = 0; i < N_SHOTS; ++i) {
        int a = prop_a.get_value(foo).to_int();
        benchmark::DoNotOptimize(a);
    }
}

void assign_via_direct_access() {
    static Foo foo{};

    for (int i = 0; i < N_SHOTS; ++i) {
        foo.a = i;
    }
}

void assign_via_map() {
    static std::map<const char*, int> foo{{"a", 42}};

    for (int i = 0; i < N_SHOTS; ++i) {
        foo["a"] = i;
    }
}

void assign_via_rttr() {
    static Foo foo{};

    for (int i = 0; i < N_SHOTS; ++i) {
        rttr::type::get(foo).set_property_value("a", i);
    }
}

void assign_via_rttr_caching() {
    static Foo foo{};
    rttr::property prop_a = rttr::type::get(foo).get_property("a");

    for (int i = 0; i < N_SHOTS; ++i) {
        prop_a.set_value(foo, i);
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
BM_TEST(access_via_map);
BM_TEST(access_via_rttr);
BM_TEST(access_via_rttr_caching);
BM_TEST(assign_via_direct_access);
BM_TEST(assign_via_map);
BM_TEST(assign_via_rttr);
BM_TEST(assign_via_rttr_caching);

BENCHMARK_MAIN();

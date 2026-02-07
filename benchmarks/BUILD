load("//:common_copts.bzl", "copts", "linkopts") 
load("//benchmarks:generate_benchmarks.bzl", "generate_benchmarks")

deps = [
    "//:lib",
    "//third_party/rttr_bazel:rttr_core",
    "@google_benchmark//:benchmark"
]
benchmark_glob_matcher = "BM_*.cpp"
benchmark_names = generate_benchmarks(benchmark_glob_matcher, deps, [])
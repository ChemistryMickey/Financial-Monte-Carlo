load("//:common_copts.bzl", "copts", "linkopts") 
load("//benchmark:generate_benchmarks.bzl", "generate_benchmarks")

deps = [
    "//:main",
    "@google_benchmark//:benchmark"
]
benchmark_glob_matcher = "BM_*.cpp"
benchmark_names = generate_benchmarks(benchmark_glob_matcher, deps, [])

cc_binary(
    name = "everything",
    srcs = [file for file in glob([benchmark_glob_matcher])],
    copts = copts(),
    linkopts = linkopts(),
    deps = deps,
)
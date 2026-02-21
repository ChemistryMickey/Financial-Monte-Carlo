load("//:generate_tests.bzl", "generate_tests")
load("//:common_copts.bzl", "copts", "linkopts")

deps = [
    "@googletest//:gtest",
    "@googletest//:gtest_main",
    "//tools/cpp_viz:lib"
]

UT_test_glob_matcher = "UT_*.cpp"
UT_test_names = generate_tests("UT_*.cpp", deps, [])

test_suite(
    name = "cached",
    tests = UT_test_names
)

cc_test(
    name = "all",
    size = "small",
    srcs = [file for file in glob([UT_test_glob_matcher], allow_empty=True)],
    deps = deps,
    copts = copts(),
    linkopts = linkopts(),
)
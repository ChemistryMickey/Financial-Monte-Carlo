load("//:common_copts.bzl", "copts", "linkopts") 
load("//tests:generate_tests.bzl", "generate_tests")

deps = [
    "//:lib",
    "//third_party/rttr:rttr_core",
    "@cli11//:cli11",
    "@googletest//:gtest",
    "@googletest//:gtest_main"
]
test_glob_matcher = "UT_*.cpp"
test_names = generate_tests(test_glob_matcher, deps, [])

test_suite(
    name = "cached",
    tests = test_names
)

cc_test(
    name = "all",
    size = "small",
    srcs = [file for file in glob([test_glob_matcher])],
    copts = copts(),
    linkopts = linkopts(),
)
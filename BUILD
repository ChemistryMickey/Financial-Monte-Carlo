config_setting(
    name="unit_testing",
    define_values = {"UNIT_TESTING": "true"}
)
config_setting(
    name="benchmarking",
    define_values = {"BENCHMARKING": "true"}
)

load("//:common_copts.bzl", "copts", "linkopts")

cc_binary(
    name = "main",
    srcs = glob(["src/*.cpp"]) + glob(["include/*.hpp"]),
    includes = ["include"],
    copts = copts(),
    linkopts = linkopts(),
    deps = [],
    visibility= ["//visibility:public"],
    linkstatic=True
)
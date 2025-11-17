config_setting(
    name="unit_testing",
    define_values = {"UNIT_TESTING": "true"}
)
config_setting(
    name="benchmarking",
    define_values = {"BENCHMARKING": "true"}
)
config_setting(
    name="optimized",
    define_values = {"OPTIMIZED": "true"}
)

load("//:common_copts.bzl", "copts", "linkopts")

cc_library(
    name = "lib",
    srcs = glob(["src/lib/*.cpp"]) + glob(["include/*.hpp"]),
    includes = ["include"],
    strip_include_prefix = "include",
    copts = copts(),
    linkopts = linkopts(),
    deps = [
        "//third_party/rttr:rttr_core"
    ],
    visibility= ["//visibility:public"],
    linkstatic=True
)

cc_binary(
    name = "main",
    srcs = ["src/main.cpp", "//:lib"],
    copts = copts(),
    linkopts = linkopts(),
    deps = [
        "//:lib", 
        "//third_party/rttr:rttr_core",
        "@cli11//:cli11"
    ],
    linkstatic=True
)
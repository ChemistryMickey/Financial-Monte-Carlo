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

config_setting(
    name="profile",
    define_values = {"PROFILE": "true"}
)

config_setting(
    name="profile_optimized",
    define_values = {"PROFILE_OPTIMIZED": "true"}
)

load("//:common_copts.bzl", "copts", "linkopts")

cc_library(
    name = "lib",
    srcs = glob([
        "src/**/*.cpp",
        "src/**/*.hpp",
        "include/*.hpp"
    ]),
    includes = ["include", "src/lib"],
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
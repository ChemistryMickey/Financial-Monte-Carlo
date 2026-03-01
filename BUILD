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

proto_library(
    name = "protos",
    srcs = glob(["src/protos/*.proto"])
)

cc_proto_library(
    name = "proto_cpps",
    deps = [":protos"],
    visibility = ["//visibility:public"]
)

cc_library(
    name = "lib",
    srcs = glob([
        "src/lib/**/*.cpp",
        "src/lib/**/*.hpp",
        "include/*.hpp"
    ]),
    includes = ["include", "src/lib"],
    strip_include_prefix = "include",
    copts = copts(),
    linkopts = linkopts(),
    deps = [
        "//third_party/rttr_bazel:rttr_core",
        "@cli11//:cli11",
        "@magic_enum//:magic_enum",
        ":proto_cpps"
    ],
    visibility= ["//visibility:public"],
    linkstatic=True
)

cc_binary(
    name = "main",
    srcs = ["src/main.cpp", "src/CliArgs.hpp", "//:lib"],
    copts = copts(),
    linkopts = linkopts(),
    deps = [
        "//:lib", 
        "//third_party/rttr_bazel:rttr_core",
        "@cli11//:cli11",
        "@magic_enum//:magic_enum"
    ],
    linkstatic=True
)
config_setting(
    name="unit_testing",
    values={
        # enabled by --define flag  in .bazelrc
        "define": "UNIT_TESTING=true",
    },
)

# compile with profiling flags. Adds -g to
config_setting(
    name="profile",
    values={
        # enabled by --define flag  in .bazelrc
        "define": "PROFILE=true",
    },
)

load("//:common_copts.bzl", "copts", "linkopts")

cc_binary(
    name = "main",
    srcs = glob(["src/*.cpp"]) + glob(["include/*.hpp"]),
    includes = ["include"],
    copts = copts(),
    linkopts = linkopts(),
    deps = [],
    visibility= ["//visibility:public"]
)
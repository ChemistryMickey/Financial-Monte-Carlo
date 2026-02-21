load("//:common_copts.bzl", "copts", "linkopts")

cc_library(
    name = "lib",
    srcs = glob(["src/lib/**/*.cpp"]),
    hdrs = glob(["src/lib/**/*.hpp"]),
    includes = ["src/lib"],
    copts = copts(),
    linkopts = linkopts(),
    deps = [],
    visibility= ["//visibility:public"],
    linkstatic=True
)

cc_binary(
    name = "main",
    srcs = [
        "src/main.cpp",
    ],
    copts = copts(),
    linkopts = linkopts() + ["-lGL"],
    deps = [
        ":lib",
        "@implot//:implot",
        "@imgui//:imgui",
        "@imgui//backends:platform-glfw",
        "@imgui//backends:renderer-opengl3",
        "@glfw//:glfw"
    ],
    linkstatic=True
)
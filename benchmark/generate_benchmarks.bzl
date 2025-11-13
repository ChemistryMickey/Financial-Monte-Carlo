load("//:common_copts.bzl", "copts", "linkopts", "file_stem_from_name")

def generate_benchmarks(file_format, deps, disabled_files):
    benchmark_files = native.glob([file_format])
    benchmark_names = []

    for bm_file in benchmark_files:
        if bm_file in disabled_files:
            continue

        bm_name = file_stem_from_name(bm_file)
        native.cc_binary(
            name = bm_name,
            srcs = [bm_file],
            deps = deps,
            copts = copts(),
            linkopts = linkopts()
        )

        benchmark_names.append(bm_name)

    return benchmark_names
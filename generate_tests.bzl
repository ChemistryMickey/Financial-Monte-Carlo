load("//:common_copts.bzl", "copts", "linkopts", "file_stem_from_name")

def generate_tests(file_format, deps, disabled_files):
    test_files = native.glob([file_format], allow_empty=True)
    test_names = []

    for file in test_files:
        if file in disabled_files:
            continue

        stem = file_stem_from_name(file)
        native.cc_test(
            name = stem,
            size = "small",
            srcs = [file],
            deps = deps,
            copts = copts(),
            linkopts = linkopts()
        )

        test_names.append(stem)

    return test_names
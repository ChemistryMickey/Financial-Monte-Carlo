load("//:common_copts.bzl", "copts", "linkopts", "file_stem_from_name")

def generate_tests(file_format, deps, disabled_files):
    unit_test_files = native.glob([file_format])
    unit_test_names = []

    for ut_file in unit_test_files:
        if ut_file in disabled_files:
            continue

        ut_name = file_stem_from_name(ut_file)
        native.cc_test(
            name = ut_name,
            size = "small",
            srcs = [ut_file],
            deps = deps,
            copts = copts(),
            linkopts = linkopts()
        )

        unit_test_names.append(ut_name)

    return unit_test_names
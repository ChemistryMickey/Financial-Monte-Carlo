def copts():
    copts = [
        # C++ standard
        "-std=c++23",
        # Debugging
        "-ggdb3",
        # Compiler warnings
        "-Wall",
        "-Wpedantic",
        "-Wextra",
        "-Wshadow",
        "-Wswitch-enum",
        "-Wnull-dereference",
        "-Wconversion",
        "-Wformat=2",

        # Features
        "-fopenmp",

        # Optimizer and code generation.
        #"-fwrapv",

        # Csv support
        "-Wno-stringop-truncation",
    ]

    copts += select({
        "//:unit_testing": [
            "-fsanitize=address,undefined,leak",
            "-fno-sanitize-recover",
            # "-Werror",
            "-DUNIT_TESTING",
        ],
        "//:benchmarking": [
            "-O3",
            "-DBENCHMARKING",
            "-DOPTIMIZE"
        ],
        "//:optimized": [
            "-O3",
            "-DOPTIMIZE"
        ],
        "//:profile": [
            "-fno-omit-frame-pointer",
            "-g"
        ],
        "//:profile_optimized": [
            "-O3",
            "-fno-omit-frame-pointer",
            "-g",
            "-DOPTIMIZE"
        ],
        "//conditions:default": []
    })

    return copts

def linkopts():
    linkopts =  [
        "-lstdc++fs",
        "-lstdc++",
        "-lm",
        "-ldl",
        "-fuse-ld=lld",
        "-fopenmp",
    ]

    linkopts += select({
        "//:unit_testing": [
            "-fsanitize=address,undefined,leak",
            "-fno-sanitize-recover",
            # "-Werror",
        ],
        "//:benchmarking": [
            "-flto",
            "-O3"
        ],
        "//:optimized": [
            "-flto",
            "-O3"
        ],
        "//:profile": [
            "-fno-omit-frame-pointer",
            "-g"
        ],
        "//:profile_optimized": [
            "-O3",
            "-flto",
            "-fno-omit-frame-pointer",
            "-g"
        ],
        "//conditions:default": []
    })

    return linkopts

def filter_files(candidate_file_list, filter_file_list):
    """given a list of input files A, return a subset of A that do not contain strings in list B

    For any element in `candidate_file_list` that contains a string element from `filter_file_list`, that element
    will not be returned from this function

    Args:
        candidate_file_list: list[str] total list of files to pass to a job, usually generated with glob()
        filter_file_list: list[str] strings that are banned from being in `candidate_file_list`

    Returns:
        list[str] subset of `candidate_file_list` not matching anything in `filter_file_list`
    """

    okay_files = []

    for candidate in candidate_file_list:
        remove = False
        for skip_file in filter_file_list:
            if skip_file in candidate:
                remove = True
                break

        if not remove:
            okay_files.append(candidate)

    return okay_files

def file_stem_from_name(unit_test_file_name):
    """Given the name of a file, return the name of the test for that file

    Args:
        unit_test_file_name (str):
            file name, ex "UT_foo.cpp"

    Returns (str):
        name of the test, ex "UT_foo"
    """

    # get the filename from the unit test. should not contain more than one '.' character
    file_parts = unit_test_file_name.split('.')
    if (len(file_parts) != 2):
        fail("Unexpected file name: %s" % unit_test_file_name)

    # the unit test name came before the . character
    unit_test_name = file_parts[0]

    return unit_test_name

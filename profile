#! /usr/bin/bash
optimize="--config=profile"
config_path="$PROJECT_HOME/data/single_run_example.json"
output_path="default"
dry=""
runs=1
verbose=""
while [[ $# -ne 0 ]]; do
    case "$1" in
        --release|-r)
            optimize="--config=profile-optimized"
            shift 1
            ;;
        --dry)
            dry="--dry"
            shift 1
            ;;
        --verbose|-v)
            verbose="--verbose"
            shift 1
            ;;
        --config)
            config_path="$PROJECT_HOME/$2"
            shift 2
            ;;
        --runs)
            runs=$2
            shift 2
            ;;
        -o|--output)
            output_path="$2"
            shift 2
            ;;
        *)
            echo "Unrecognized command! $1"
            echo "./profile [--release|-r] [--config $config_path] [--output|-o $output_path] [--dry] [--runs $runs]"
            exit 1
            ;;
    esac
done

set -xe

bazel build //:main $optimize
flamegraph --flamechart -- --no-kernel $PROJECT_HOME/bazel-bin/main --config $config_path $dry --runs $runs -o $output_path $verbose
rm $PROJECT_HOME/perf.*

# Finance Monte Carlo

A Monte Carlo simulation of various financial projections which include things like:
- Stocks / Bonds
- Medical/inheritance events
- Inflation
- Asset value appreciation/depreciation

The goal of this simulation is to demonstrate the following:
- Clever logging without C++26 (reflection isn't supported even in gcc-15)
- A simulation scheduler
- Monte Carlo variable dispersions
- Bazel 8
- C++23 funsies
- Comprehensive unit testing
- Benchmark capabilities for high-performance

This is also meant to "put your money where your mouth is" w.r.t. simulation architecture and understanding that it's not going to be correct the first time.
This is an attempt to get the wrong decisions out of the way.

## Testing
Unit test names are automatically generated from each of the UT_*.cpp files in the tests directory.

In order to run any given test, you can execute the following:
```bash
bazel test //tests:UT_$your_desired_test --config=unit_testing
```
or you can execute them all like so:
```bash
bazel test //tests:cached --config=unit_testing
```
This will execute them all sequentially.
You may additionally specify a number of threads with the `-j N` option to `bazel test`

## Benchmarking
Benchmark names are automatically generated from each of the BM_*.cpp files in the benchmark directory.

In order to run any given benchmark, you can execute the following:
```bash
bazel run //benchmark:BM_$your_desired_benchmark --config=benchmarking
```
or you can execute them all like so:
```bash
bazel run //benchmark:everything --config=benchmarking
```
This will execute all benchmarks sequentially.
It's not recommended to multithread the benchmarks to prevent performance degradation as a result of resource competition.

This `--config=benchmarking` is to enable optimizations (because the optimized performance is the more important metric).

That said, omitting this config switch is sufficient to test using an unoptimized build if curiosity strikes.
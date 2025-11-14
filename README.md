# Finance Monte Carlo

A Monte Carlo simulation of various financial projections which include things like:
- Stocks / Bonds
- Medical/inheritance events
- Inflation
- Asset value appreciation/depreciation

The goal of this simulation is to demonstrate the following:
- [ ] Clever logging without C++26 (reflection isn't supported even in gcc-15)
- [ ] A simulation scheduler
- [ ] Some form of reflection? Or some way to runtime access properties via string (e.g. a map)
- [/] Monte Carlo variable dispersions
- [x] Bazel 8
- [ ] .[C++23 funsies](https://youtu.be/vbHWDvY59SQ)
    - I'm just stoked for `std::print`
- [x] Comprehensive unit testing
- [x] Benchmark capabilities for high-quality performance decisions

This is also meant to "put your money where your mouth is" w.r.t. simulation architecture and understanding that it's not going to be correct the first time.
This is an attempt to get the wrong decisions out of the way.

## General Design


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

Alternatively, the simplest way is to just run `./test $desired_test`

## Benchmarking
Benchmark names are automatically generated from each of the BM_*.cpp files in the benchmark directory.

In order to run any given benchmark, you can execute the following:
```bash
bazel run //benchmark:BM_$your_desired_benchmark --config=benchmarking
```

This `--config=benchmarking` is to enable optimizations (because the optimized performance is the more important metric).

That said, omitting this config switch is sufficient to test using an unoptimized build if curiosity strikes.

Alternatively, the simplest way is to just run `./benchmark $desired_benchmark`

## Known issues
- Super annoying that RTTR throws a ton of warnings such that I can't use `-Werror`. Not gonna worry about that now tho.
# Finance Monte Carlo

A Monte Carlo simulation of various financial projections which include things like:
- Stocks / Bonds
- Medical events
- Inflation / Deflation events
- Asset value appreciation/depreciation

The goal of this simulation is to demonstrate the following:
- [x] All C++ (except for plotting and some BASH plumbing)
- [x] Clever logging without C++26 (reflection isn't supported even in gcc-15)
    - Went with [RTTR](https://www.rttr.org/)
- [x] CLI parsing
    - Using [CLI11](https://cliutils.github.io/CLI11/book/)
- [x] Some form of reflection (in particular for logging and dispersions)? Or some way to runtime access properties via string (e.g. a map)
    - Went with [RTTR](https://www.rttr.org/)
- [x] Monte Carlo variable dispersions
    - Should probably have made diff files rather than entire configs for each MC run, but doing so would have required I had reflection up and running and I didn't want that system to block the dispersion system.
- [x] Bazel 8
- [x] .[C++23 funsies](https://youtu.be/vbHWDvY59SQ)
    - It's pretty slick.
- [x] "Pretty comprehensive" unit testing. This isn't an enterprise project though.
- [x] Benchmark capabilities for high-quality performance decisions
- [x] Disperse a large number of potential variables and evaluate an overall probability of success w/ statistical enclosures to demonstrate that confidence.

This is also meant to "put your money where your mouth is" w.r.t. simulation architecture and understanding that it's not going to be correct the first time.
This is an attempt to get the wrong decisions out of the way.

## Testing
Unit test names are automatically generated from each of the UT_*.cpp files in the tests directory.

In order to run the tests, there's an included CLI utility, `test`, such that one need only run 
```bash
./test Logger
```
to build and run the `UT_Logger.cpp` unit test.

To run the whole suite, one need only run 
```bash
./test
```

## Benchmarking
Benchmark names are automatically generated from each of the BM_*.cpp files in the benchmark directory.

In order to run a benchmark, there's an included CLI utility, `benchmark`, such that one need only run 
```bash
./benchmark rttr_vs_map
```
to build and run the Google Benchmark RTTR reflection vs Map.

There is no option to run all benchmarks because these are expensive.

## TODO / Known Bugs (in Prio order)
- [ ] Seems like sometimes, Money's cents can be negative and that borks everything
- [ ] Integration testing using the `Person`
- [ ] Tune the dispersions better. Right now the median stock market goes down which seems wrong. Inflation also tends deflationary which seems wrong.
- [ ] Add a bond market to supplement the stock market. Maybe also a non-liquid asset market such as art/long-term investments?
- [ ] Come up with a more clever way of seeding the RNGs than "42"

## Design flaws / Kinda jank
- Adding a dispersion is very manual. Gotta add it to the object config parse, then add it to the SimConfig.cpp parse.
- Adding a loggable is also quite manual. It can probably be wrapped in a couplea macros.
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

## Running
### Requirements
- Bazel (8.4.2 at time of writing, but refer to the .bazelversion file)
- g++
- C++23
- `uv` (Python package management for viz)
- `perf` (for profiling)

### Running
This should probably be streamlined via the Dockerfile but it doesn't currently work.

#### Setup
1. (if you want to visualize) Activate your Python venv: `source .venv/bin/activate`
1. Make sure that `PROJECT_HOME` (envvar) is defined: `source envvars.sh`
1. (if you need to push to GitHub) Activate your ssh agent: `eval "$(ssh-agent -s)"`, `ssh-add ~/.ssh/<key_file>`

#### Single Run
1. Compile/run the single-run: `./run`

You may additionally specify `--visualize` to output png plots of the plotted signals. 
This requires the `uv` venv to be setup though.

There's additionally a `--release` flag to build with optimizations but the single run is so fast it's probably not worth doing.

#### Monte Carlo
1. (if you want to run a Monte Carlo) Run the Monte Carlo: `./run --runs <desired_n_runs> --config $PROJECT_HOME/data/sim_config.json --visualize`
    1. `--runs` specifies the number of runs you'd like (duh)
    1. `--config` specifies the configuration input file (in this case, `sim_config.json` is a dispersion definition JSON which is necessary for Monte Carlos)
    1. `--visualize` specifies that you'd like to output an HTML report containing plots and analysis after the Monte Carlo completes. `--visualize` is also available for a single run, but it's more powerful in a Monte Carlo context.

There's additionally a `--release` flag to build with optimizations. 
A single-threaded 1k MC running over 65 years takes about 45 seconds optimized at time of writing.

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
- [ ] Integration testing using the `Person`. There's probably some jank in there w.r.t. updates and deciding when to enter and exit stock positions.
- [ ] Tune the dispersions better. Right now the median stock market goes down which seems wrong. Inflation also tends deflationary which seems wrong.
- [ ] Add separate "failed run" section in MC report to further investigate reasons why you went bankrupt.
- [ ] Add a bond market to supplement the stock market. Maybe also a non-liquid asset market such as art/long-term investments?
- [ ] "Simulation Controller" such that `run_simulation` is controlled in a more "registration" manner than a bespoke function.
- [ ] Come up with a more clever way of seeding the RNGs than "42"

## Design flaws / Kinda jank
- Adding a dispersion is very manual. Gotta add it to the object config parse, then add it to the SimConfig.cpp parse.
- Adding a loggable is also quite manual. It can probably be wrapped in a couplea macros.
- Setup/envvars is kinda jank.

## Lessons Learned
- RTTR is nice but I don't think it's significantly more ergonomic than something like a Loggable struct that defines a series of `std::map<std::string, std::reference_wrapper<T>>` which each model would have and then a `TimeseriesDataLogger` which takes a map of these loggables. Then, at registration time, the TimeseriesDataLogger just has a small map of those enabled signals rather than the master map of all the possible loggable signals. The only potential downside I see to this scheme is that the maps would increase the size of the model objects by N reference wrapper sizes (which is pretty negligable?).
- The CLI11 parsing is nice! I'll deffo be using this in future projects.
- Bazel 8's MODULE.bazel is much more convenient than setting the http_library (or whatever) from earlier bazel versions.
- C++23 has some very nice ergonomic improvements (ranges, print).
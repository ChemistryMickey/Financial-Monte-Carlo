# Finance Monte Carlo

A Monte Carlo simulation of various financial projections which include things like:
- Stocks / Bonds
- Medical/inheritance events
- Inflation
- Asset value appreciation/depreciation

The goal of this simulation is to demonstrate the following:
- [ ] All C++ (except for plotting and some BASH plumbing)
- [x] Clever logging without C++26 (reflection isn't supported even in gcc-15)
    - Went with RTTR
- [ ] A simulation scheduler
- [ ] CLI parsing
- [x] Some form of reflection (in particular for logging and dispersions)? Or some way to runtime access properties via string (e.g. a map)
    - Went with RTTR
- [x] Monte Carlo variable dispersions
    - Should probably have made diff files rather than entire configs for each MC run, but doing so would have required I had reflection up and running and I didn't want that system to block the dispersion system.
- [x] Bazel 8
- [ ] .[C++23 funsies](https://youtu.be/vbHWDvY59SQ)
    - I'm just stoked for `std::print`. Mmk, `std::print` may be over-hyped if it can't print addresses.
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

## Scratch
- Looks like reference wrappers on a map of variable names may be the way to go w.r.t. logging things? [godbolt](https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGIAKwAnKSuADJ4DJgAcj4ARpjEIGZmpAAOqAqETgwe3r4BwemZjgLhkTEs8YnJtpj2JQxCBEzEBLk%2BfkG19dlNLQRl0XEJSSkKza3t%2BV3j/YMVVaMAlLaoXsTI7BzmAMwRyN5YANQmO24sTKmn2CYaAIK7%2B4eYJ2dUXgwO2WLXtw9me0%2Bz1ebic42ImFYv3uEQIRwuEQgSxMAHYrPcjpijug1rF6EcmKQjrEicgiehTui7lijuN0CAUGs4ac3CyTsk7uh0BCFAojqgqASQOyzCC2eYAGxMMVnEUmfxuBjmMyUv40ukMtBeZlncUcrk8vkC4nC5UytzsiWxc1yhVK5KqjFYjWM7U25Wc7mYXn8wXIU3Jd1mCXIIMqu3Kx3U50EemunWs2Ueg3eo2C%2BkioMS9Bh%2BWKyM7Knq2MMi5XM6wokuiFUBKMTYAfQA7sRLqkEiycV48ZhfjtsPDUUWaVihxoiUxUQARUhqkeYodcImxaezp3zocpI6mFEzucbtE7cnT/cnXdR09dnuSgkNmuvKfwgB0TAIEA0SyfwEwb%2BRhb%2Bp4ulqCZ6mYnqGr6QqZiyepSmGE4pj6xpMHemBUAGoowUmwYofeWEWpGEYOv%2B9ynvwxAQEw2qoDe8oWHgRIAG7yo%2BwosMiaKnsWcbAWG4GppBZr4UceBhhhWaMV%2BP5IrmREqiR0YLuepG7hwKy0Jw/i8H4HBaKQqCcKyljWLSawbC8uw8KQBCaGpKwANYBOOGkcJI2m2fpnC8AoIDjjZulqaQcCwEgaAsKkdAJOQlBhRF9CJMACjMKkCgIKgBB8HQBAJD5ECxB5sQRC0ACenBWYVzDEMVADysTaJgDhlbwYVsII1UMLQpUBaQWCxF4wBuGItA%2BdwvBYBcRjiN1%2BAQl8jHeh5mCqA12pbHpsJ1B5tB4LErZVR4WAeQQxB4CwTWkPNxCxBkmBTpgE3ANtRi2SsVAGIlABqeCYE21XtjpVn8IIIhiOwUgyIIigqOo3W6EuBjPaYxmWPoO0%2BZAKyoKkDQjQAtLjdKnFOSNWJYAK8Kgl0nVg6NIt0DUNC4DDuJ4HR6GEERDJUIxLkUWQCFMfi8xk/MMPMwyJEudgM70ExtKz%2BRS3UMsCH0rTi9zku2HLgt6LM6ucwsPMrAoZmbBI6mae53UGRwRyqAAHBKuMSpIRzAMgoYQMdHz2UsRwQLghAkOyOxcEsvD%2BVoKwQEgTKpNq0UQLFkXEFErBbI7zuu%2B7ntHFIT5mLwmD4EQ1N6EDwiiOI4OV1DageXDpAtpcTWWxwWmkDpem29V2oJ3CxpZy7bse17PsMH7AceOFqeh%2BHkcvQ5Tn6Jwbldx5tveXoUd2avHBFxvNtedZS8XTl2QgJIQA)
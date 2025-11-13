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
#include "Percentiles.hpp"
#include <vector>
#include <algorithm>
#include <cmath>

namespace fmc {
    double percentile(std::vector<double>& v, double p) {
        std::sort(v.begin(), v.end());
        double idx = p * (double) (v.size() - 1);
        size_t lo = static_cast<size_t>(floor(idx));
        size_t hi = static_cast<size_t>(ceil(idx));
        double w = idx - (double) lo;
        return v[lo] * (1.0 - w) + v[hi] * w;
    }

    PercentileStats computeStats(const std::vector<std::vector<double>>& allRuns) {
        PercentileStats stats;
        size_t num_runs = allRuns[0].size();

        stats.p5.reserve(num_runs);
        stats.p25.reserve(num_runs);
        stats.p50.reserve(num_runs);
        stats.p75.reserve(num_runs);
        stats.p95.reserve(num_runs);
        stats.max.reserve(num_runs);
        stats.min.reserve(num_runs);

        for (size_t run_idx = 0; run_idx < num_runs; ++run_idx) {
            std::vector<double> sample;
            sample.reserve(allRuns.size());

            for (const auto& run : allRuns) {
                sample.push_back(run[run_idx]);
            }

            stats.min[run_idx] = percentile(sample, 0.0);
            stats.p5[run_idx] = percentile(sample, 0.05);
            stats.p25[run_idx] = percentile(sample, 0.25);
            stats.p50[run_idx] = percentile(sample, 0.50);
            stats.p75[run_idx] = percentile(sample, 0.75);
            stats.p95[run_idx] = percentile(sample, 0.95);
            stats.max[run_idx] = percentile(sample, 1.0);
        }

        return stats;
    }

}
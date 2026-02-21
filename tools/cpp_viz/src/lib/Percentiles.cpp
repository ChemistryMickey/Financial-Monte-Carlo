#include "Percentiles.hpp"
#include <vector>
#include <algorithm>
#include <cmath>

namespace fmc {
    double Percentile(std::vector<double>& v, double p) {
        std::sort(v.begin(), v.end());
        double idx = p * (double) (v.size() - 1);
        size_t lo = static_cast<size_t>(floor(idx));
        size_t hi = static_cast<size_t>(ceil(idx));
        double w = idx - (double) lo;
        return v[lo] * (1.0 - w) + v[hi] * w;
    }

    PercentileStats ComputeStats(const std::vector<std::vector<double>>& allRuns) {
        PercentileStats stats;
        size_t N = allRuns[0].size();

        stats.p5.reserve(N);
        stats.p25.reserve(N);
        stats.p50.reserve(N);
        stats.p75.reserve(N);
        stats.p95.reserve(N);
        stats.max.reserve(N);
        stats.min.reserve(N);

        for (size_t i = 0; i < N; ++i) {
            std::vector<double> sample;
            sample.reserve(allRuns.size());

            for (const auto& run : allRuns) {
                sample.push_back(run[i]);
            }

            stats.min[i] = Percentile(sample, 0.0);
            stats.p5[i] = Percentile(sample, 0.05);
            stats.p25[i] = Percentile(sample, 0.25);
            stats.p50[i] = Percentile(sample, 0.50);
            stats.p75[i] = Percentile(sample, 0.75);
            stats.p95[i] = Percentile(sample, 0.95);
            stats.max[i] = Percentile(sample, 1.0);
        }

        return stats;
    }

}
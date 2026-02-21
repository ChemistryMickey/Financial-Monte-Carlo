#pragma once
#include <vector>

namespace fmc {
    struct PercentileStats {
        std::vector<double> p5, p25, p50, p75, p95, min, max;
    };

    double Percentile(std::vector<double>& v, double p);

    PercentileStats ComputeStats(const std::vector<std::vector<double>>& allRuns);

}
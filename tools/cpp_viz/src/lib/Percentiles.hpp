#pragma once
#include <vector>

namespace fmc {
    /// @brief The percentile statistics of given data calculated from computeStats
    struct PercentileStats {
        std::vector<double> p5, p25, p50, p75, p95, min, max;
    };

    /// @brief Calculate the percentile, p, of a given vector v.
    /// @note A CSV is ingested row-major but statistics are calculated by column.
    double percentile(std::vector<double>& v, double p);

    /// @brief Compute the statistics for a CSV.
    PercentileStats computeStats(const std::vector<std::vector<double>>& allRuns);

}
#pragma once
#include <vector>
#include <string>

namespace fmc {
    struct TimeSeriesFile {
        std::vector<double> time;
        std::vector<std::string> columnNames;
        std::vector<std::vector<double>> columns;
    };

    TimeSeriesFile loadCsv(const std::string& path);
}
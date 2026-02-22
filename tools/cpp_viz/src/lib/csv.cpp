#include "csv.hpp"
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fmc {
    static inline std::string trim(const std::string& s) {
        auto start = s.find_first_not_of(" \t\r\n");
        auto end = s.find_last_not_of(" \t\r\n");
        if (start == std::string::npos) return "";
        return s.substr(start, end - start + 1);
    }

    TimeSeriesFile loadCsv(const std::string& path) {
        TimeSeriesFile result;
        std::ifstream file(path);
        std::string line;

        if (!file.is_open()) return result;

        // Read header
        std::getline(file, line);
        std::stringstream header(line);
        std::vector<std::string> headers;
        {
            std::string cell;

            while (std::getline(header, cell, ',')) {
                headers.push_back(cell);
            }
        }

        // First column is time
        result.columnNames.assign(headers.begin() + 1, headers.end());
        size_t expected_columns = 0;

        while (std::getline(file, line)) {
            line = trim(line);
            if (line.empty()) continue;

            std::stringstream ss(line);
            std::string cell;
            std::vector<double> row;

            while (std::getline(ss, cell, ',')) {
                cell = trim(cell);
                try {
                    row.push_back(std::stod(cell));
                }
                catch (const std::exception&) {
                    throw std::runtime_error("Invalid numeric value in CSV: " + cell);
                }
            }

            if (row.empty())
                continue;

            if (expected_columns == 0) {
                expected_columns = row.size();
                if (expected_columns < 1)
                    throw std::runtime_error("CSV must have at least one column.");
                result.columns.resize(expected_columns - 1);
            }
            else if (row.size() != expected_columns) {
                throw std::runtime_error("Inconsistent number of columns in CSV.");
            }

            // First column = time
            result.time.push_back(row[0]);

            // Remaining columns = data
            for (size_t i = 1; i < row.size(); ++i) {
                result.columns[i - 1].push_back(row[i]);
            }
        }
        return result;
    }

}
#pragma once

#include <fstream>
#include <format>
#include <iostream>
#include <string>
#include <utility>
#include <sstream>
#include <vector>
#include <filesystem>
#include "json.hpp"

#include "utils.hpp"
#include "math.hpp"

// File handling

// Text files

// JSON files
inline nlohmann::json read_json(const std::filesystem::path& filename) {
    std::ifstream f{project_path(filename)};
    if (!f.is_open()) {
        throw std::runtime_error(std::format("Unable to open file {}", project_path(filename).string()));
    }

    nlohmann::json j;
    f >> j;

    return j;
}

inline void write_json(const std::filesystem::path& filename, const nlohmann::json& data) {
    constexpr size_t indent_width = 3;

    std::ofstream f{project_path(filename)};

    if (f.is_open()) {
        f << data.dump(indent_width);
    }
    else {
        throw std::runtime_error(std::format("Unable to open file {}", project_path(filename).string()));
    }
}

// CSVs
template <Numeric T = double>
inline void write_csv(const std::filesystem::path& filename, const std::vector<std::pair<std::string, std::vector<T>>>& dataset) {
    // Open and check file
    std::ofstream f(project_path(filename));
    if (f.fail()) {
        std::cout << "Failed to open file " << filename << "!\n";
        throw std::runtime_error("Failed to open file!");
    }

    if (dataset.size() == 0) {
        return;
    }

    // Sanity check that the column sizes match
    size_t start_column_size = dataset.at(0).second.size();
    for (size_t i = 1; i < dataset.size(); ++i) {
        if (dataset.at(i).second.size() != start_column_size) {
            throw std::runtime_error("Column sizes in CSV write don't match!");
        }
    }

    /// TODO: Make this append if the columns are already present
    // Send columns to CSV
    for (uint j = 0; j < dataset.size(); ++j) {
        f << dataset.at(j).first;
        if (j != dataset.size() - 1) {
            f << ",";
        }
    }
    f << "\n";

    // Send data to file
    for (uint i = 0; i < start_column_size; ++i) {
        for (uint j = 0; j < dataset.size(); ++j) {
            f << dataset.at(j).second.at(i);
            if (j != dataset.size() - 1) f << ",";
        }
        f << "\n";
    }
}

template<Numeric T = double>
inline std::vector<std::pair<std::string, std::vector<T>>> read_csv(const std::filesystem::path& filename) {
    std::vector<std::pair<std::string, std::vector<T>>> result;

    std::ifstream f(project_path(filename));

    if (!f.is_open()) throw std::runtime_error("Couldn't open file!");

    std::string line, colname;
    T val;

    if (f.good()) {
        std::getline(f, line);
        std::stringstream ss(line);

        while (std::getline(ss, colname, ',')) {
            result.push_back({colname, std::vector<T>{}});
        }
    }

    while (std::getline(f, line)) {
        std::stringstream ss(line);

        size_t col_i = 0;

        while (ss >> val) {
            result.at(col_i).second.push_back(val);

            // Ignore commas
            if (ss.peek() == ',') ss.ignore();

            ++col_i;
        }
    }

    return result;
}
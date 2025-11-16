#pragma once
#include <filesystem>
#include <CLI/CLI.hpp>

namespace fmc {
    struct CliArgs {
        std::filesystem::path out_directory = "output";
        std::filesystem::path config_path;
        uint runs = 1;
        bool dry = false;
        bool verbose = false;

        CliArgs(int argc, char** argv) {
            // Required options
            app.add_option("--config", this->config_path, "The configuration file required to run the simulation")->required();

            // Optional options
            app.add_flag("--dry", this->dry, "Just generate dispersion configs");
            app.add_option("--output,-o", this->out_directory, "The output directory relative to $PROJECT_HOME/output");
            app.add_option("--runs", this->runs, "Number of runs to execute");

            // Parse it up
            app.parse(argc, argv);
        }

    private:
        CLI::App app{"A Monte Carlo simulator for financial stability"};
    };
}
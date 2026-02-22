#pragma once
#include <filesystem>
#include "CLI/CLI.hpp"
#include "utils.hpp"

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
            app.add_flag("--verbose,-v", this->verbose, "Set logging to DEBUG and logger buffer to 1 (useful if you think you're missing logs)");
            app.add_option("--output,-o", this->out_directory, "The output directory relative to $PROJECT_HOME");
            app.add_option("--runs", this->runs, "Number of runs to execute");

            // Parse it up
            app.parse(argc, argv);

            this->out_directory = project_path(this->out_directory);
        }

    private:
        CLI::App app{"A Monte Carlo simulator for financial stability"};
    };
}

namespace std {
    template<>
    struct formatter<fmc::CliArgs> {
        constexpr auto parse(format_parse_context& ctx) {
            return ctx.begin(); // no custom format specifiers
        }

        template <typename FormatContext>
        auto format(const fmc::CliArgs& args, FormatContext& ctx) const {
            return std::format_to(
                ctx.out(),
                "Args:\n\t"
                "Config Path: {}\n\t"
                "Output: {}\n\t"
                "Runs: {}\n\t"
                "Dry: {}\n\t"
                "Verbose: {}",
                args.config_path,
                args.out_directory,
                args.runs,
                args.dry,
                args.verbose
            );
        }
    };
} // namespace std

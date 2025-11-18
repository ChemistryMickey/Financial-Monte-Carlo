#include "Logger.hpp"
#include "file_io.hpp"
#include "CliArgs.hpp"
#include "SimConfig.hpp"
#include "run_simulation.hpp"


int main(int argc, char** argv) {
    fmc::CliArgs args{argc, argv};
    fmc::Logger::instance().initialize(args.out_directory, 100, args.verbose);

    DEBUG("\n{}", args);

    auto config_json = read_json(args.config_path);
    DEBUG("Read sim config JSON");

    std::filesystem::remove_all(args.out_directory);
    if (args.runs == 1) {
        // Then this is a single run and the JSON should be for a single shot without dispersions
        std::filesystem::create_directories(args.out_directory);

        fmc::SimConfig config{config_json};
        DEBUG("Parsed config JSON into SimConfig");

        run_simulation(args.out_directory, config);
    }
    else {
        fmc::generate_dispersed_configs(args.out_directory, args.runs, config_json);
        if (args.dry) {
            return 0;
        }

        // This should be parallelized at some point but this is pretty darn fast.
        for (uint run = 0; run < args.runs; ++run) {
            INFO("Beginning run {}", run);

            std::filesystem::path run_dir = run_out_dir(args.out_directory, run);
            fmc::SimConfig config{read_json(run_dir / "monte_config.lock")};
            run_simulation(run_dir, config);
        }
    }
}
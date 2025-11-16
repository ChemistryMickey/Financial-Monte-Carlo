#include <print>

#include "json.hpp"
#include "include/Logger.hpp"
#include "include/utils.hpp"
#include "include/file_io.hpp"
#include "include/Person.hpp"
#include "include/CliArgs.hpp"

fmc::Logger logger{"output/output.log"};

int main(int argc, char** argv) {
    INFO("Logger successfully initialized");
    fmc::CliArgs args{argc, argv};

    if (args.dry) {
        INFO("Performing dry run");
        return 0;
    }

    auto sim_config = read_json(project_path("data/sim_config.json"));
    // auto dispersed_sim_config =
    //     auto single_config_from_dispersion(sim_config["person"]);
    fmc::Person person{sim_config["person"]};
    INFO(std::format("Person starting money: {}", sim_config["person"]["starting_money"]["initial_undispersed_value"].get<double>()));
}
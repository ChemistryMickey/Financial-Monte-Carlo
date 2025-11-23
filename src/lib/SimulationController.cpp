#include "SimulationController.hpp"
#include "utils.hpp"
#include "Logger.hpp"

namespace fmc {
    void SimulationController::run() {
        for (auto& model : this->models) {
            model.get().initialize();
        }

        // Simulation loop
        while (cur_date < end_date) {
            DEBUG("Starting day {}", cur_date);
            // January 1st updates
            if (is_jan_1st(cur_date)) {
                for (auto& model : models) {
                    model.get().yearly_update();
                }
            }

            // Environment
            for (auto& model : models) {
                model.get().environment(DT);
            }

            // Terminal Update
            for (auto& model : models) {
                model.get().update(DT);
            }

            // Log state
            ts_logger.log();

            // Termination conditions
            for (auto& model : models) {
                if (model.get().should_terminate()) {
                    WARN("Terminating on {}", cur_date);
                    return;
                }
            }

            // Increment date
            cur_date += std::chrono::days{DT};
        }

    }
}
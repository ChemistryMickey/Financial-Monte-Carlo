#pragma once
#include "models/Model.hpp"
#include "TimeseriesDataLogger.hpp"
#include <vector>
#include <chrono>

namespace fmc {
    constexpr uint DT = 1;

    struct SimulationController {
        std::chrono::sys_days cur_date;
        std::chrono::sys_days end_date;
        std::vector<std::reference_wrapper<Model>> models;
        TimeseriesDataLogger& ts_logger;

        SimulationController(
            std::chrono::sys_days& clock_,
            const std::chrono::sys_days& end_date_,
            const std::vector<std::reference_wrapper<Model>>& models_,
            TimeseriesDataLogger& ts_logger_) :
            cur_date{clock_},
            end_date{end_date_},
            models{models_},
            ts_logger{ts_logger_} {}

        void run();
    };
}
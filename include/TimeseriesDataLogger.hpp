#pragma once
#include <functional>
#include <rttr/type>
#include "utils.hpp"

namespace fmc {
    class Loggable {
        RTTR_ENABLE();
    };

    class TimeseriesDataLogger {
        std::chrono::sys_days& clock;
        std::map<std::string, std::reference_wrapper<Loggable>> registered_objs;
        std::vector<std::string> signals_to_log;
    };
}
#pragma once

#include <rttr/type>
#include <rttr/registration_friend>

#include "include/json.hpp"
#include "Bond.hpp"
#include "Model.hpp"
#include "Bond.hpp"
#include "TimeseriesDataLogger.hpp"

namespace fmc {
    class BondMarket : public TimeseriesLoggable, public Model {
    public:
        BondMarket(const nlohmann::json& config);
        void initialize() override {}
        void update(unsigned int) override {}
        void environment(unsigned int) override {}
        void yearly_update() override {}

        bool should_terminate() override { return false; };

        RTTR_ENABLE(TimeseriesLoggable);
        RTTR_REGISTRATION_FRIEND;
    };
}
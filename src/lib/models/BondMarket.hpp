#pragma once

#include <rttr/type>
#include <rttr/registration_friend>
#include <unordered_map>

#include "include/json.hpp"
#include "Bond.hpp"
#include "Model.hpp"
#include "Bond.hpp"
#include "TimeseriesDataLogger.hpp"

namespace fmc {
    class BondMarket : public TimeseriesLoggable, public Model {
    public:
        std::unordered_map<SecurityType, double> security_interest_rates{
            {SecurityType::T_Bill, 0.0},
            {SecurityType::T_Note, 0.0},
            {SecurityType::TIPS, 0.0}
        };

        BondMarket(const nlohmann::json& config);
        void initialize() override {}
        void update(unsigned int) override {}
        void environment(unsigned int) override;
        void yearly_update() override {}

        bool should_terminate() override { return false; };

        RTTR_ENABLE(TimeseriesLoggable);
        RTTR_REGISTRATION_FRIEND;
    };
}
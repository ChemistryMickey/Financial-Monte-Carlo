#pragma once

#include <rttr/type>
#include <rttr/registration_friend>
#include <unordered_map>

#include "include/json.hpp"
#include "Bond.hpp"
#include "Model.hpp"
#include "Money.hpp"
#include "Bond.hpp"
#include "math.hpp"
#include "TimeseriesDataLogger.hpp"

namespace fmc {
    class BondMarket : public TimeseriesLoggable, public Model {
    public:
        /// @brief Interest rates for the various security types
        std::unordered_map<SecurityType, double> security_interest_rates{
            {SecurityType::T_Bill, 0.0},
            {SecurityType::T_Note, 0.0},
            {SecurityType::T_IPS, 0.0}
        };
        std::unordered_map<SecurityType, double> security_volatility{
            {SecurityType::T_Bill, 0.0},
            {SecurityType::T_Note, 0.0},
            {SecurityType::T_IPS, 0.0}
        };
        std::unordered_map<SecurityType, Money> security_face_value{
            {SecurityType::T_Bill, Money{0.0}},
            {SecurityType::T_Note, Money{0.0}},
            {SecurityType::T_IPS, Money{0.0}}
        };

        /// @brief The current simulation day such that this can check if a bond should mature/coupon pay out
        std::chrono::sys_days cur_day;

        /// @brief Random Number Generator for interest rate volatility
        RNG rng;

        BondMarket(const nlohmann::json& config, const std::chrono::sys_days& start_date);
        void initialize() override {}
        void update(unsigned int) override {}
        void environment(unsigned int) override;
        void yearly_update() override {}
        bool should_terminate() override { return false; };

        /// @brief If any bonds are ready to mature, mark them as matured. It's then up to the caller to pay out the maturation value and remove them from the vector.
        void set_bonds_maturity(std::vector<Bond>& bond_vec);

        /// @brief Determine the coupon payment of this time tick. Also mutates the bond such that the next coupon payment is modified.
        Money calc_coupon_payments(std::vector<Bond>& bond_vec);

        RTTR_ENABLE(TimeseriesLoggable);
        RTTR_REGISTRATION_FRIEND;
    };
}
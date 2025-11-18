#pragma once
#include "utils.hpp"
#include "math.hpp"
#include "json.hpp"

namespace fmc {
    template<Numeric T>
    struct Event {
        /// @brief On a given day, how likely are you to encounter this event [0, 1]?
        ClampedValue<T> probability = {0, {0, 1}};

        /// @brief The RNG used to determine if this event occurs on a given day
        RNG rng{};

        /// @brief Year on year, what's the geometric increase in events [0, \inf]? For example, in a medical sense this is aging.
        ClampedValue<T> scaling_factor = {0, {0, std::numeric_limits<double>::max()}};

        /// @brief If you encounter this event, what's the price for it [0, \inf] (before inflation)?
        ClampedValue<T> cost = {0, {0, std::numeric_limits<double>::max()}};

        Event() = default;
        Event(const nlohmann::json& config) {
            this->probability = {config.at("probability").get<double>(), this->probability.get_limits()};
            this->scaling_factor = {config.at("scaling_factor").get<double>(), this->scaling_factor.get_limits()};
            this->cost = {config.at("initial_cost").get<double>(), this->cost.get_limits()};

            this->rng.reseed(config.at("rng_seed").get<uint>());
        }

        /// @brief Has this event occurred?
        /// @return true if the event has occurred and it should be handled. false if the event hasn't occurred
        bool occurred() {
            return this->rng.uniform_real(0, 1) < this->probability.get_value();
        }

        void update(uint days_passed) {
            this->probability = this->probability.get_value() * (this->scaling_factor.get_value() * (1.0 + (double) days_passed / (double) 365));
        }
    };
}
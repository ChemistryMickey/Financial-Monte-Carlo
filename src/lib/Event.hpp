#pragma once
#include <chrono>
#include "rttr/type"
#include "rttr/registration_friend"

#include "TimeseriesDataLogger.hpp"

#include "utils.hpp"
#include "math.hpp"
#include "json.hpp"

namespace fmc {
    struct Event : public TimeseriesLoggable {
        /// @brief On a given day, how likely are you to encounter this event [0, 1]?
        ClampedValue<double> probability = {0, {0, 1}};

        /// @brief Don't want knockdown to drop you below your original probability of an event.
        ClampedValue<double> initial_probability = {0, {0, 1}};

        /// @brief Year on year, what's the geometric increase in events [0, \inf]? For example, in a medical sense this is aging.
        ClampedValue<double> scaling_factor = {0, {0, std::numeric_limits<double>::max()}};

        /// @brief When an event occurs, by how much should the probability of it happening again be changed (+ is increasing prob, - is decreasing)?
        ClampedValue<double> knockdown = {0.0, {-1, 1}};

        /// @brief The last time this event occurred. Incremented each update. Reset when event procs.
        uint days_since_last_proc = 0;

        /// @brief How long does this event last? For example, a cancer diagnosis might result in small, consistent medical costs. 0 is "instantaneous" (just on the day it procs).
        uint duration = 0;

        /// @brief When this event procs, what's the shortest time (in days) before it can proc again?
        uint cooldown = 0;

        /// @brief If you encounter this event, what's the effect of it?
        double effect_val = 0.0;

        /// @brief The seed for the RNG object
        uint rng_seed = 42;

        /// @brief The RNG used to determine if this event occurs on a given day
        RNG rng{};

        Event() {
            this->rng.reseed(this->rng_seed);
        }
        static Event from_json(const nlohmann::json& config);

        /// @brief Has this event occurred?
        /// @return true if the event has occurred and it should be handled. false if the event hasn't occurred
        bool occurred();

        /// @brief Is this event currently in progress?
        /// @return true if the event is in progress (and therefore DoTs)
        bool in_progress();

        /// @brief Is this event currently on cooldown?
        /// @return true if the event is on cooldown, false if it can be re-proced
        bool on_cooldown();

        void update(uint days_passed);

        /// @brief Set the days_since_last_proc to a value such that it can naturally trip.
        void arm_event();

        RTTR_ENABLE(TimeseriesLoggable);
        RTTR_REGISTRATION_FRIEND;

        // This allows us to immediately serialize and deserialize an Event.
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Event, cooldown, duration, effect_val, knockdown, probability, scaling_factor, rng_seed);
    };
}


namespace std {
    template <>
    struct formatter<fmc::Event> {
        constexpr auto parse(format_parse_context& ctx) {
            return ctx.begin();
        }

        template <typename FormatContext>
        auto format(const fmc::Event& e, FormatContext& ctx) const {
            return format_to(ctx.out(), "");
        }
    };
} // namespace std
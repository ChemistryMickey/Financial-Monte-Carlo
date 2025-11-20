#pragma once
#include <chrono>
#include "rttr/type"
#include "rttr/registration_friend"

#include "TimeseriesDataLogger.hpp"

#include "utils.hpp"
#include "math.hpp"
#include "json.hpp"

/// TODO:
///     - Cooldowns (events shouldn't be able to happen back to back)
///     - Actually unit test this because I'm not sure the update is doing what you expect

namespace fmc {
    class Event : public TimeseriesLoggable {
        /// @brief On a given day, how likely are you to encounter this event [0, 1]?
        ClampedValue<double> probability = {0, {0, 1}};

        /// @brief Year on year, what's the geometric increase in events [0, \inf]? For example, in a medical sense this is aging.
        ClampedValue<double> scaling_factor = {0, {0, std::numeric_limits<double>::max()}};

        /// @brief The last time this event occurred. Incremented each update.
        uint last_occurred = 0;

        /// @brief How long does this event last? For example, a cancer diagnosis might result in some
        uint duration = 1;

        /// @brief When an event occurs, by how much should the probability of it happening again be reduced?
        double knockdown = 0.0;

        /// @brief The RNG used to determine if this event occurs on a given day
        RNG rng{};

    public:
        Event() = default;
        Event(const nlohmann::json& config);

        /// @brief If you encounter this event, what's the effect of it?
        double effect_val = 0.0;

        /// @brief Has this event occurred?
        /// @return true if the event has occurred and it should be handled. false if the event hasn't occurred
        bool occurred();

        void update(uint days_passed);

        RTTR_ENABLE(TimeseriesLoggable);
        RTTR_REGISTRATION_FRIEND;

        friend struct std::formatter<Event>;
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
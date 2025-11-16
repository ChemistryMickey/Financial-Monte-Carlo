#pragma once
#include <numeric>
#include <vector>

#include "math.hpp"
#include "json.hpp"
#include "Money.hpp"

namespace fmc {
    class Person {
        /// @brief Reference to the stock market from which asset prices will be calculated
        // StockMarket& stock_market;

        /// @brief The amount of money currently in your wallet; not tied up in assets
        Money cash_on_hand{0.0};

        /// @brief The current portfolio of stocks
        // std::vector<SimpleStock> stock_portfolio{};

        /// @brief The stock/bond ratio you try to maintain each day [0, 1]. 1 means "all stocks"
        ClampedValue<double> stock_bond_ratio{0, {0, 1}};

        /// @brief In a given year, how likely are you to have an adverse medical event that costs money [0, 1]?
        ClampedValue<double> medical_event_probability = {0, {0, 1}};

        /// @brief Year on year, what's the geometric increase in medical events [0, \inf]? Basically, aging.
        ClampedValue<double> medical_event_scaling_factor = {0, {0, std::numeric_limits<double>::max()}};

        /// @brief If you have a medical event, what's the price for it [0, \inf] (before inflation)?
        ClampedValue<double> medical_event_initial_cost = {0, {0, std::numeric_limits<double>::max()}};

    public:
        // Person(StockMarket& stock_market_, const nlohmann::json& config);
        Person(const nlohmann::json& config);

        /// @brief The current net worth of the individual (assets and cash)
        Money current_net_worth() const;

        /// @brief At the end of the simulation frame, update all relevant values
        void update();

        friend struct std::formatter<Person>;
    };
}

template<>
struct std::formatter<fmc::Person> : std::formatter<std::string> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const fmc::Person& p, FormatContext& ctx) {
        return std::format_to(ctx.out(), "Person:\n\tNet Worth: {}\n\tCash on hand: {}\n\tStock/Bond Ratio: {}",
            p.current_net_worth(),
            p.cash_on_hand,
            p.stock_bond_ratio
        );
    }
};
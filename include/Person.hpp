#pragma once
#include <numeric>
#include <vector>

#include "math.hpp"
#include "json.hpp"
#include "Money.hpp"
#include "Event.hpp"

namespace fmc {
    class Person {
        /// @brief Reference to the stock market from which asset prices will be calculated
        // StockMarket& stock_market;

        // BondMarket& bond_market;

        // AnnualInflation& annual_inflation;

        /// @brief The amount of money currently in your wallet; not tied up in assets
        Money cash_on_hand{0.0};

        /// @brief The amount you expect to spend every year without special events (your "burn rate") before inflation
        Money yearly_expenses{0.0};

        /// @brief The current portfolio of stocks
        // std::vector<SimpleStock> stock_portfolio{};

        /// @brief The stock/bond ratio you try to maintain each day [0, 1]. 1 means "all stocks"
        ClampedValue<double> stock_bond_ratio{0, {0, 1}};

        Event<double> medical_event{};

    public:
        // Person(StockMarket& stock_market_, BondMarket& bond_market_, const nlohmann::json& config);
        Person() = default;
        Person(const nlohmann::json& config);

        /// @brief Move money from cash on hand to stocks/bonds before starting the simulation loop
        void initialize();

        /// @brief Update the person based on the status of the stock and bonds market. Also calculates events (e.g. medical) and subtracts from cash on hand.
        /// Additionally updates the probability of a medical event and applies inflation
        void update(uint days_passed);

        /// @brief The current net worth of the individual (assets and cash)
        Money current_net_worth() const;

        // Allows access to private members
        friend struct std::formatter<Person>;
    };
}

namespace std {
    template <>
    struct formatter<fmc::Person> {
        constexpr auto parse(format_parse_context& ctx) {
            return ctx.begin();
        }

        template <typename FormatContext>
        auto format(const fmc::Person& p, FormatContext& ctx) const {
            return format_to(ctx.out(), "Person:\n\tNet Worth: {}\n\tCash on hand: {}\n\tYearly Expenses: {}\n\tStock/Bond Ratio: {}\n\tMedical Event probability: {}",
                p.current_net_worth(),
                p.cash_on_hand,
                p.yearly_expenses,
                p.stock_bond_ratio.to_string(),
                p.medical_event.probability.to_string()
            );
        }
    };
} // namespace std
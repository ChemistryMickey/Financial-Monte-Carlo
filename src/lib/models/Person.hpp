#pragma once
#include <numeric>
#include <vector>
#include <rttr/type>
#include <rttr/registration_friend>

#include "math.hpp"
#include "json.hpp"
#include "Money.hpp"
#include "Event.hpp"
#include "TimeseriesDataLogger.hpp"
#include "StockMarket.hpp"
#include "AnnualInflation.hpp"

namespace fmc {
    class Person : public TimeseriesLoggable {
        /// @brief Reference to the stock market from which asset prices will be calculated
        StockMarket& stock_market;

        // BondMarket& bond_market;

        AnnualInflation& annual_inflation;

        /// @brief The amount of money currently in your wallet; not tied up in assets
        Money cash_on_hand{0.0};

        /// @brief Only buy stock if you're above this thresold. Don't want to expose yourself to too much risk.
        Money desired_cash_on_hand{0.0};

        /// @brief The amount you expect to spend every year without special events (your "burn rate") before inflation
        Money yearly_expenses{0.0};

        /// @brief The amount of money you make per year until you retire
        Money income_until_retirement{0.0};

        /// @brief The percent your income increases per year (ticks on January 1st for simplicity)
        ///
        ///     After all, how much you make depends mostly on how much you made before and the strongest predictor of how much you'll make is how much your parents made.
        double yearly_raise_percent{0.0};

        /// @brief How old is this person?
        uint current_age = 30;

        /// @brief At what age will they retire (at which point they stop collecting their income)?
        uint retirement_age = 75;

        /// @brief At what age do you expect to die?
        uint death_age = 90;

        /// @brief The current portfolio of stocks
        // std::vector<SimpleStock> stock_portfolio{};

        /// @brief The stock/bond ratio you try to maintain each day [0, 1]. 1 means "all stocks"
        ClampedValue<double> stock_bond_ratio{0, {0, 1}};

        Event medical_event{};

    public:
        // Person(StockMarket& stock_market_, BondMarket& bond_market_, const nlohmann::json& config);
        Person(StockMarket& stock_market_, AnnualInflation& annual_inflation_, const nlohmann::json& config);

        /// @brief Move money from cash on hand to stocks/bonds before starting the simulation loop
        void initialize();

        /// @brief Update the person based on the status of the stock and bonds market. Also calculates events (e.g. medical) and subtracts from cash on hand.
        /// Additionally updates the probability of a medical event and applies inflation
        void update(uint days_passed);

        /// @brief At the top of the year, some things need to be proc-ed. Basically guaranteed events.
        void yearly_update();

        Money value_in_stock() const;

        bool dead() const {
            return this->current_age >= this->death_age;
        }

        // Termination conditions
        /// @brief Takes into account the value of stocks and bonds.
        Money current_net_worth{0.0};
        // Why not a uint? I don't want a negative wrap round issue
        int n_stocks = 0;

        // Formatting and RTTR things
        // Allows access to private members
        friend struct std::formatter<Person>;

        RTTR_ENABLE(TimeseriesLoggable);
        RTTR_REGISTRATION_FRIEND;
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
                p.current_net_worth,
                p.cash_on_hand,
                p.yearly_expenses,
                p.stock_bond_ratio.to_string(),
                p.medical_event.probability.to_string()
            );
        }
    };
} // namespace std
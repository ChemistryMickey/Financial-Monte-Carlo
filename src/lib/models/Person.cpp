#include "Person.hpp"
#include "Logger.hpp"
#include <rttr/registration>

RTTR_REGISTRATION{
    rttr::registration::class_<fmc::Person>("Person")
        .property_readonly("cash_on_hand", &fmc::Person::cash_on_hand) (
            // This means the value will be returned as a reference wrapper such that I can keep it in a table for logging! Slick.
            rttr::policy::prop::as_reference_wrapper
        )
        .property_readonly("yearly_expenses", &fmc::Person::yearly_expenses) (
            rttr::policy::prop::as_reference_wrapper
        )
        .property_readonly("n_stocks", &fmc::Person::n_stocks) (
            rttr::policy::prop::as_reference_wrapper
        )
        .property_readonly("current_net_worth", &fmc::Person::current_net_worth) (
            rttr::policy::prop::as_reference_wrapper
        );
}

namespace fmc {
    Person::Person(
        StockMarket& stock_market_,
        BondMarket& bond_market_,
        AnnualInflation& annual_inflation_,
        const nlohmann::json& config) :
        stock_market{stock_market_},
        bond_market{bond_market_},
        annual_inflation{annual_inflation_} {

        this->cash_on_hand = config.at("starting_money").get<double>();
        this->yearly_expenses = config.at("initial_yearly_expenses").get<double>();
        this->stock_bond_ratio = {config.at("stock_bond_ratio").get<double>(), this->stock_bond_ratio.get_limits()};
        this->medical_event = Event{config.at("medical_event")};
        this->income_until_retirement = config.at("income_until_retirement").get<double>();
        this->yearly_raise_percent = config.at("yearly_raise_percent").get<double>();
        this->desired_cash_on_hand = config.at("desired_cash_on_hand").get<double>();
        this->current_age = config.at("current_age").get<uint>();
        this->retirement_age = config.at("retirement_age").get<uint>();
        this->death_age = config.at("death_age").get<uint>();
    }

    Money Person::value_in_stock() const {
        return this->stock_market.position_price * this->n_stocks;
    }

    Money Person::value_in_bonds() const {
        Money bond_values{0.0};
        for (auto& bond : this->bonds) {
            bond_values += bond.face_value;
        }

        return bond_values;
    }

    void Person::initialize() {
        // Drop cash on stocks according to the desired proportion
        Money cash_for_stocks = this->cash_on_hand.to_double() * this->stock_bond_ratio.get_value();
        Money cash_for_bonds = this->cash_on_hand.to_double() * (1 - this->stock_bond_ratio.get_value());

        this->n_stocks = (int) (cash_for_stocks / this->stock_market.position_price.to_double());
        this->cash_on_hand -= this->stock_market.position_price * this->n_stocks;

        this->buy_bonds(cash_for_bonds);
    }

    void Person::buy_bonds(const Money& cash) {
        DEBUG("Buying bonds");
        /// TODO: Buy other kinds of bonds, not just T_BILLs
        /// TODO: Buy a more complex T_BILL portfolio rather than just 1 Year bonds.
        size_t n_bonds = (size_t) std::floor(cash / this->bond_market.security_face_value[SecurityType::T_Bill]);
        for (size_t n = 0; n < n_bonds; ++n) {
            this->bonds.push_back(this->bond_market.buy_bond(BondDuration_days::TwelveMonth));
            this->cash_on_hand -= this->bonds.back().face_value;
        }

        DEBUG("Purchased {} bonds", this->bonds.size());
    }

    void Person::yearly_update() {
        this->current_age += 1;
        this->income_until_retirement *= (1 + this->yearly_raise_percent);

        if (this->current_age == this->retirement_age) {
            INFO("Retiring at {}. I hope your money doesn't run out.", this->current_age);
        }
    }

    bool Person::should_terminate() {
        // Termination Conditions
        if (this->bankrupt()) {
            INFO("Went bankrupt with a net worth of ${} and {} stocks ({} $/share)",
                this->current_net_worth,
                this->n_stocks,
                stock_market.position_price
            );
            return true;
        }

        if (this->dead()) {
            INFO("Died with ${} net worth at {} years old", this->current_net_worth, this->current_age);
            return true;
        }

        return false;
    }

    void Person::update(uint days_passed) {
        // Event handling
        if (this->medical_event.occurred()) {
            // Then unfortunately yes, subtract the value from the cash on hand
            this->cash_on_hand = this->cash_on_hand - this->medical_event.effect_val;
            DEBUG("Medical event. Previous cash: {}, New cash: {}, Event: {}",
                this->cash_on_hand + this->medical_event.effect_val,
                this->cash_on_hand,
                this->medical_event
            );
        }

        // Subtract off daily expenses
        Money daily_expense{this->yearly_expenses.to_double() / 365.0};
        this->cash_on_hand -= daily_expense * days_passed;

        // Add income
        if (this->current_age < this->retirement_age) {
            Money daily_income{this->income_until_retirement.to_double() / 365.0};
            this->cash_on_hand += daily_income * days_passed;
        }

        // Update bonds
        this->bond_market.set_bonds_maturity(this->bonds);
        for (size_t i = this->bonds.size() - 1; i > 0; --i) {
            auto& bond = this->bonds.at(i);
            if (bond.should_mature) {
                DEBUG("Bond Matured:\n{}", bond);
                this->current_net_worth += bond.maturation_value;
                this->bonds.erase(this->bonds.begin() + i);
            }
        }

        // Given cash on hand, liquidate assets or invest
        const Money excess_cash = this->cash_on_hand - this->desired_cash_on_hand;
        const Money stock_price = this->stock_market.position_price;
        const Money stock_available_cash = excess_cash * this->stock_bond_ratio.get_value();
        int64_t n_stock_to_buy = (int64_t) std::floor(stock_available_cash / stock_price);

        const Money bond_face_value = this->bond_market.security_face_value[SecurityType::T_Bill];
        const Money bond_available_cash = (excess_cash * (1 - this->stock_bond_ratio.get_value()));
        int64_t n_bonds_to_buy = (int64_t) std::floor(bond_available_cash / bond_face_value);

        // Now buy stock
        if (n_stock_to_buy > 0) {
            this->n_stocks += n_stock_to_buy;

            this->cash_on_hand -= stock_price * n_stock_to_buy;
        }

        // Now buy bonds
        if (n_bonds_to_buy > 0) {
            DEBUG("Purchasing {} bonds on {}", n_bonds_to_buy, this->bond_market.cur_day);
            this->bonds.push_back(this->bond_market.buy_bond(BondDuration_days::TwelveMonth));
        }

        // Now make sure you're solvent
        bool sufficient_cash_on_hand = this->cash_on_hand > this->desired_cash_on_hand;
        if (!sufficient_cash_on_hand && this->n_stocks > 0) {
            int64_t stock_to_sell = std::min(
                (int64_t) ((this->desired_cash_on_hand - this->cash_on_hand) / stock_price) + 1,
                this->n_stocks);
            this->n_stocks -= stock_to_sell;

            this->cash_on_hand += stock_price * stock_to_sell;
            DEBUG("Liquidating {} stock at ${}. New cash on hand: ${}", stock_to_sell, stock_price, this->cash_on_hand);
        }

        if (this->cash_on_hand < Money{0.0}) {
            // Liquidate until you're solvent
            int64_t stock_to_sell = std::min((int64_t) (this->cash_on_hand / stock_price) + 1, this->n_stocks);
            this->n_stocks -= stock_to_sell;

            this->cash_on_hand += stock_price * stock_to_sell;
            DEBUG("Dissolving {} stock at ${}. New cash on hand: ${}", stock_to_sell, stock_price, this->cash_on_hand);
        }


        // Inflation Influenced Updates
        double inflation_multiplier = (1 + this->annual_inflation.annual_inflation_rate / 365.0);

        // Update yearly expenses and desired cash on hand based on inflation
        this->yearly_expenses *= inflation_multiplier;
        this->desired_cash_on_hand *= inflation_multiplier;

        // Update medical event probability and value
        this->medical_event.effect_val *= inflation_multiplier;

        // Event updates
        this->medical_event.update(days_passed);
        this->current_net_worth = this->cash_on_hand + this->value_in_stock();
    }
}
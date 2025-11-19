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
    Person::Person(StockMarket& stock_market_, const nlohmann::json& config) :
        stock_market{stock_market_} {
        DEBUG("Constructing Person with config {}", config.dump(3));

        this->cash_on_hand = config.at("starting_money").get<double>();
        this->yearly_expenses = config.at("initial_yearly_expenses").get<double>();
        this->stock_bond_ratio = {config.at("stock_bond_ratio").get<double>(), this->stock_bond_ratio.get_limits()};
        this->medical_event = Event<double>{config.at("medical_event")};

        DEBUG("Constructed Person");
    }

    Money Person::value_in_stock() const {
        return this->stock_market.position_price * this->n_stocks;
    }

    void Person::initialize() {
        // Until you have a bond market, just drop all your cash in stocks.
        this->n_stocks = (int) (this->cash_on_hand.to_double() / this->stock_market.position_price.to_double());
        this->cash_on_hand -= this->stock_market.position_price * this->n_stocks;
    }

    void Person::update(uint days_passed) {
        // Did a medical event happen?
        // double medical_event_v = this->medical_event.rng.uniform_real(0, 1);
        // if (medical_event_v < this->medical_event.probability.get_value()) {
        if (this->medical_event.occurred()) {
            // Then unfortunately yes, subtract the value from the cash on hand
            this->cash_on_hand = this->cash_on_hand - this->medical_event.cost.get_value();
            DEBUG("Medical event. Previous cash: {}, New cash: {}, Medical event probability: {}",
                this->cash_on_hand + this->medical_event.cost.get_value(),
                this->cash_on_hand,
                this->medical_event.probability.get_value()
            );
        }

        // Subtract off daily expenses
        Money daily_expense{this->yearly_expenses.to_double() / 365.0};
        this->cash_on_hand -= daily_expense;

        // Given cash on hand, liquidate assets or invest where you can
        if (this->cash_on_hand < Money{0.0}) {
            // Liquidate until you're solvent
            double stock_price = this->stock_market.position_price.to_double();
            int stock_to_sell = -this->cash_on_hand.to_double() / stock_price + 1;
            this->n_stocks -= stock_to_sell;

            DEBUG("Liquidating {} stock at ${}", stock_to_sell, stock_price);
            this->cash_on_hand += stock_price * stock_to_sell;
        }

        // Update yearly expenses (just a simple linear update for now. A buck a day)
        this->yearly_expenses = this->yearly_expenses + days_passed;

        // Update medical event probability
        this->medical_event.update(days_passed);

        // Update your net worth
        this->current_net_worth = this->cash_on_hand + this->value_in_stock();
    }
}
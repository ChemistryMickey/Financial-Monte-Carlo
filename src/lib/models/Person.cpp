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

    Money Person::current_net_worth() const {
        return this->cash_on_hand;
    }

    void Person::initialize() {

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

        // Update asset prices

        // Given cash on hand, liquidate assets or invest where you can

        // Update yearly expenses (just a simple linear update for now. A buck a day)
        this->yearly_expenses = this->yearly_expenses + days_passed;

        // Update medical event probability
        this->medical_event.update(days_passed);
    }
}
#include "Person.hpp"
#include "Logger.hpp"

namespace fmc {
    Person::Person(const nlohmann::json& config) {
        this->cash_on_hand = config["starting_money"].get<double>();
        this->yearly_expenses = config["initial_yearly_expenses"].get<double>();
        this->stock_bond_ratio = {config["stock_bond_ratio"].get<double>(), this->stock_bond_ratio.get_limits()};
        this->medical_event = Event<double>{config.at("medical_event")};
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

        // Update medical event probability
        this->medical_event.update(days_passed);
    }
}
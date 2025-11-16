#include "Person.hpp"

namespace fmc {
    Person::Person(const nlohmann::json& config) {
        this->cash_on_hand = config["starting_money"].get<double>();
        this->stock_bond_ratio = {config["stock_bond_ratio"].get<double>(), this->stock_bond_ratio.get_limits()};
        this->medical_event_probability = {config["medical_event_probability"].get<double>(), this->medical_event_probability.get_limits()};
        this->medical_event_scaling_factor = {config["medical_event_scaling_factor"].get<double>(), this->medical_event_scaling_factor.get_limits()};
        this->medical_event_initial_cost = {config["medical_event_initial_cost"].get<double>(), this->medical_event_initial_cost.get_limits()};
    }

    Money Person::current_net_worth() const {
        return this->cash_on_hand;
    }
}
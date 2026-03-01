#include "Event.hpp"
#include "Logger.hpp"

#include "rttr/registration"

RTTR_REGISTRATION{
    rttr::registration::class_<fmc::Event>("Event")
        .property_readonly("effect_val", &fmc::Event::effect_val);
}

namespace fmc {
    Event Event::from_json(const nlohmann::json& j) {
        Event e = j;
        e.rng.reseed(e.rng_seed);
        e.arm_event();
        return e;
    }

    void Event::arm_event() {
        this->days_since_last_proc = this->cooldown + this->duration;
    }

    bool Event::in_progress() {
        return this->days_since_last_proc < this->duration;
    }

    bool Event::on_cooldown() {
        return this->days_since_last_proc < this->cooldown;
    }

    /// @brief Has this event occurred?
    /// @return true if the event has occurred and it should be handled. false if the event hasn't occurred
    bool Event::occurred() {
        bool dot_proc = this->in_progress();
        if (this->on_cooldown()) {
            return dot_proc;
        }

        bool natural_proc = this->rng.uniform_real(0, 1) < this->probability.get_value();

        if (natural_proc) {
            this->days_since_last_proc = 0;

            double proposed_probability = this->probability.get_value() + this->knockdown.get_value();
            if (proposed_probability < this->initial_probability.get_value()) {
                this->probability = this->initial_probability;
            }
            else {
                this->probability.set_value(proposed_probability);
            }
        }

        return natural_proc || dot_proc;
    }

    void Event::update(uint days_passed) {
        this->probability *= (this->scaling_factor.get_value() * (1.0 + (double) days_passed / (double) 365));
        this->days_since_last_proc += days_passed;
    }
}
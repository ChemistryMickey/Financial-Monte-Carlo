#include "Event.hpp"
#include "rttr/registration"

RTTR_REGISTRATION{
    rttr::registration::class_<fmc::Event>("Event")
        .property_readonly("effect_val", &fmc::Event::effect_val);
}

namespace fmc {
    Event::Event(const nlohmann::json& config) {
        this->probability = {config.at("probability").get<double>(), this->probability.get_limits()};
        this->scaling_factor = {config.at("scaling_factor").get<double>(), this->scaling_factor.get_limits()};
        this->effect_val = config.at("event_val").get<double>();
        this->knockdown = config.at("knockdown").get<double>();
        this->duration = config.at("duration").get<uint>();
        this->cooldown = config.at("cooldown").get<uint>();

        this->rng.reseed(config.at("rng_seed").get<uint>());
    }

    /// @brief Has this event occurred?
    /// @return true if the event has occurred and it should be handled. false if the event hasn't occurred
    bool Event::occurred() {
        bool on_cooldown = this->days_since_last_proc < this->cooldown;
        bool dot_proc = this->days_since_last_proc < this->duration;
        if (on_cooldown) {
            return dot_proc;
        }

        bool natural_proc = this->rng.uniform_real(0, 1) < this->probability.get_value();

        if (natural_proc) {
            this->days_since_last_proc = 0;
            this->probability += this->knockdown;
        }

        return natural_proc || dot_proc;
    }

    void Event::update(uint days_passed) {
        this->probability *= (this->scaling_factor.get_value() * (1.0 + (double) days_passed / (double) 365));
        this->days_since_last_proc += days_passed;
    }
}
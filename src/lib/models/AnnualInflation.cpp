#include "AnnualInflation.hpp"
#include "Logger.hpp"
#include "json.hpp"

#include <rttr/registration>

RTTR_REGISTRATION{
    rttr::registration::class_<fmc::AnnualInflation>("AnnualInflation")
        .property_readonly("annual_inflation_rate", &fmc::AnnualInflation::annual_inflation_rate) (
            rttr::policy::prop::as_reference_wrapper
        );
}

namespace fmc {
    AnnualInflation::AnnualInflation(const nlohmann::json& config) :
        annual_inflation_rate{config.at("rate").get<double>()},
        inflation_event(config.at("inflation_event")),
        deflation_event(config.at("deflation_event")) {}

    void AnnualInflation::update(uint days_passed) {
        if (this->inflation_event.occurred()) {
            DEBUG("Inflation event proc-ed");
            this->annual_inflation_rate += this->inflation_event.effect_val;

            // Inflation sparks deflation
            this->deflation_event.probability.set_value(
                this->deflation_event.probability.get_value() + this->inflation_event.probability.get_value()
            );
        }

        if (this->deflation_event.occurred()) {
            DEBUG("Deflation event proc-ed");
            this->annual_inflation_rate += this->deflation_event.effect_val;

            // Deflation sparks deflation
            this->inflation_event.probability.set_value(
                this->deflation_event.probability.get_value() + this->inflation_event.probability.get_value()
            );
        }

        this->inflation_event.update(days_passed);
        this->deflation_event.update(days_passed);
    }
}
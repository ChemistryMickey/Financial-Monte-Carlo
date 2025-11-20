#include "StockMarket.hpp"
#include "Logger.hpp"
#include <rttr/registration>

RTTR_REGISTRATION{
    rttr::registration::class_<fmc::StockMarket>("StockMarket")
        .property_readonly("position_price", &fmc::StockMarket::position_price) (
            // This means the value will be returned as a reference wrapper such that I can keep it in a table for logging! Slick.
            rttr::policy::prop::as_reference_wrapper
        );
}

namespace fmc {
    StockMarket::StockMarket(const nlohmann::json& config) :
        position_price{config.at("position_price").get<double>()},
        boom_scaling_event(config.at("boom_scaling_event")),
        bust_scaling_event(config.at("bust_scaling_event")),
        volatility{config.at("volatility").get<double>()},
        annual_time_scaling_factor{config.at("annual_time_scaling_factor").get<double>()},
        rng{config.at("rng_seed").get<uint>()} {}

    void StockMarket::update(uint days_passed) {
        double effective_time_scaling_factor = this->annual_time_scaling_factor;
        if (boom_scaling_event.occurred()) {
            DEBUG("Boom StockMarket event in progress");
            effective_time_scaling_factor += this->boom_scaling_event.effect_val;
        }
        if (bust_scaling_event.occurred()) {
            DEBUG("Bust StockMarket event in progress");
            effective_time_scaling_factor += this->bust_scaling_event.effect_val;
        }

        Money scale_adjustment = this->position_price * (effective_time_scaling_factor / 365.0);
        Money volatility_adjustment = this->position_price * this->rng.normal(0, this->volatility);

        this->position_price += scale_adjustment + volatility_adjustment;

        this->boom_scaling_event.update(days_passed);
        this->bust_scaling_event.update(days_passed);
    }

    void StockMarket::initialize() {}
}
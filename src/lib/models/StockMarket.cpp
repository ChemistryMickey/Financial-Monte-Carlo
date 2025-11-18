#include "StockMarket.hpp"
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
        annual_time_scaling_factor{config.at("annual_time_scaling_factor").get<double>()},
        volatility{config.at("volatility").get<double>()},
        rng{config.at("rng_seed").get<uint>()} {}

    void StockMarket::update(uint days_passed) {

    }

    void StockMarket::initialize() {

    }
}
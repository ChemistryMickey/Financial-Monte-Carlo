#include "gtest/gtest.h"
#include "src/protos/AnnualInflationConfig.pb.h"
#include "google/protobuf/util/json_util.h"

#include "models/AnnualInflation.hpp"
#include "json.hpp"
#include <string>

struct TestAnnualInflation : ::testing::Test {
    std::string json_string = R"(
    {
        "deflation_event": {
            "cooldown": 120,
            "duration": 30,
            "event_val": -0.002,
            "knockdown": -0.30,
            "probability": 1e-05,
            "rng_seed": 42,
            "scaling_factor": 1.00001
        },
        "inflation_event": {
            "cooldown": 120,
            "duration": 365,
            "event_val": 0.001,
            "knockdown": -0.30,
            "probability": 1e-05,
            "rng_seed": 42,
            "scaling_factor": 1.00001
        },
        "rate": 0.02
    }
)";
};

TEST_F(TestAnnualInflation, ConfigCreation) {
    AnnualInflationConfig config;
    auto status = google::protobuf::util::JsonStringToMessage(json_string, &config);

    if (!status.ok()) {
        throw std::runtime_error("Failed to parse");
    }

}
#include "include/RandomVariable.hpp"
#include <stdexcept>
namespace fmc {
    double RandomVariable::next_value() {
        switch (this->distribution_type) {
            // Break not needed because of returns. Feels weird though so still mentioning it explicitly
        case DistributionType::Uniform:
            this->cur_val = this->initial_undispersed_value + this->rng.uniform_real(this->rng_args.limits.lower, this->rng_args.limits.upper);
            return this->cur_val;

        case DistributionType::Gaussian:
            this->cur_val = this->initial_undispersed_value + this->rng.normal(this->rng_args.normal.mean, this->rng_args.normal.stddev);
            return this->cur_val;

        case DistributionType::TruncatedGaussian:
            this->cur_val = this->initial_undispersed_value + this->rng.truncated_normal(this->rng_args.normal.mean, this->rng_args.normal.stddev, this->rng_args.limits.lower, this->rng_args.limits.upper);
            return this->cur_val;
        }

        throw std::runtime_error("Unexpected Distribution Type!");
    }

    RandomVariable::RandomVariable(const nlohmann::json& config) :
        initial_undispersed_value{config["initial_undispersed_value"].get<double>()},
        rng{config["seed"].get<uint>()} {
        this->cur_val = this->initial_undispersed_value;
        std::string requested_distribution_type = config["dispersion_type"].get<std::string>();
        if (requested_distribution_type == "uniform") {
            this->distribution_type = DistributionType::Uniform;
            this->rng_args = TruncatedGaussianDistributionArgs{
                .limits{
                    .lower = config["dispersion"]["lower"].get<double>(),
                    .upper = config["dispersion"]["upper"].get<double>()
                },
                .normal{}
            };
        }
        else if (requested_distribution_type == "gaussian") {
            this->distribution_type = DistributionType::Gaussian;
            this->rng_args = TruncatedGaussianDistributionArgs{
                .limits{},
                .normal {
                    .mean = config["dispersion"]["mean"].get<double>(),
                    .stddev = config["dispersion"]["standard_deviation"].get<double>()
                },
            };
        }
        else if (requested_distribution_type == "truncated_gaussian") {
            this->distribution_type = DistributionType::TruncatedGaussian;
            this->rng_args = TruncatedGaussianDistributionArgs{
                .limits{
                    .lower = config["dispersion"]["lower"].get<double>(),
                    .upper = config["dispersion"]["upper"].get<double>()
                },
                .normal {
                    .mean = config["dispersion"]["mean"].get<double>(),
                    .stddev = config["dispersion"]["standard_deviation"].get<double>()
                },
            };
        }
        else {
            throw std::runtime_error(std::format("Unrecognized dispersion type: {}", requested_distribution_type));
        }
    }
}
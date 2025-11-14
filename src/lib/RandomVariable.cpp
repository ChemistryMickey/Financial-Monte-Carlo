#include "include/RandomVariable.hpp"
#include <stdexcept>
namespace fmc {
    double RandomVariable::next_value() {
        switch (this->distribution_type) {
            // Break not needed because of returns. Feels weird though so still mentioning it explicitly
        case DistributionType::Uniform:
            this->cur_val = this->rng.uniform_real(this->rng_args.limits.lower, this->rng_args.limits.upper);
            return this->cur_val;

        case DistributionType::Gaussian:
            this->cur_val = this->rng.normal(this->rng_args.normal.mean, this->rng_args.normal.stddev);
            return this->cur_val;

        case DistributionType::TruncatedGaussian:
            this->cur_val = this->rng.truncated_normal(this->rng_args.normal.mean, this->rng_args.normal.stddev, this->rng_args.limits.lower, this->rng_args.limits.upper);
            return this->cur_val;
        }

        throw std::runtime_error("Unexpected Distribution Type!");
    }
}
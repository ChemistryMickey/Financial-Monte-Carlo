#pragma once
#include "math.hpp"
#include "test_utils.hpp"
namespace fmc {
    enum class DistributionType {
        Uniform,
        Gaussian,
        TruncatedGaussian
    };

    struct UniformDistributionArgs {
        double lower = 0;
        double upper = 0;
    };
    using Limits = UniformDistributionArgs; // These are functionally synonyms

    struct GaussianDistributionArgs {
        double mean = 0;
        double stddev = 1;
    };

    /// @brief The argments of a Truncated Gaussian Distribution. This is a superset of the Uniform and Gaussian distribution arguments.
    struct TruncatedGaussianDistributionArgs {
        Limits limits{};
        GaussianDistributionArgs normal{};
    };

    class RandomVariable {
    public:
        DistributionType distribution_type;
        double initial_undispersed_value;
        double cur_val = 0;

        TruncatedGaussianDistributionArgs rng_args{};

        RandomVariable() = delete;
        RandomVariable(double initial_undispersed_value_, Limits limits, uint rng_seed = 42) :
            distribution_type{DistributionType::Uniform},
            initial_undispersed_value{initial_undispersed_value_},
            cur_val{initial_undispersed_value},
            rng_args{.limits{limits}, .normal{}},
            rng{rng_seed} {}

        RandomVariable(double initial_undispersed_value_, GaussianDistributionArgs gauss_args, uint rng_seed = 42) :
            distribution_type{DistributionType::Gaussian},
            initial_undispersed_value{initial_undispersed_value_},
            cur_val{initial_undispersed_value},
            rng_args{.limits{}, .normal{gauss_args}},
            rng{rng_seed} {}

        RandomVariable(double initial_undispersed_value_, TruncatedGaussianDistributionArgs trunc_gauss_args, uint rng_seed = 42) :
            distribution_type{DistributionType::TruncatedGaussian},
            initial_undispersed_value{initial_undispersed_value_},
            cur_val{initial_undispersed_value},
            rng_args{trunc_gauss_args},
            rng{rng_seed} {}

        double next_value();

        TESTING_PUBLIC
            RNG rng;
    };
}
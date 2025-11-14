#pragma once

#include <cmath>
#include <random>

namespace fmc {
    inline double erfinv(double x) {
        // Constants for approximation
        const double a = 0.147;

        if (x <= -1.0) return -INFINITY;
        if (x >= 1.0) return  INFINITY;

        double ln = std::log(1 - x * x);
        double s = (2 / (M_PI * a)) + ln / 2.0;
        double t = std::sqrt(s * s - ln / a);

        // Return negative branch for x < 0
        return (x < 0 ? -1 : 1) * std::sqrt(t - s);
    }

    inline double norm_inv(double p) {
        return std::sqrt(2) * erfinv(2 * p - 1);
    }


    inline double norm_cdf(double x) {
        return 0.5 * std::erfc(-x / std::sqrt(2));
    }

    /// @brief From https://people.math.sc.edu/Burkardt/presentations/truncated_normal.pdf
    /// @param gen RNG Generator which is used for the random sampling
    /// @param mean The mean of the normal distribution from which this will be sampled
    /// @param stddev The standard deviation of the normal from which this value will be sampled
    /// @param lower The lower truncation limit of the normal
    /// @param upper The upper truncation limit of the normal
    /// @return A value sampled randomly from within the normal distribution
    inline double truncated_normal_icdf(
        std::mt19937& gen,
        double mean,
        double stddev,
        double lower,
        double upper
    ) {
        // Convert bounds to CDF space
        double a = (lower - mean) / stddev;
        double b = (upper - mean) / stddev;

        double alpha = norm_cdf(a);
        double beta = norm_cdf(b);

        // Uniform sample in [alpha, beta]
        std::uniform_real_distribution<double> U(alpha, beta);
        double u = U(gen);

        // Inverse CDF
        double z = norm_inv(u);

        return mean + stddev * z;
    }

    class RNG {
    public:
        explicit RNG(uint seed = std::random_device{}())
            : engine(seed) {}

        // Reseed the generator
        void reseed(unsigned int seed) {
            engine.seed(seed);
        }

        // Uniform real in [a, b)
        double uniform_real(double a = 0.0, double b = 1.0) {
            std::uniform_real_distribution<double> dist(a, b);
            return dist(engine);
        }

        // // Uniform integer in [a, b]
        // int uniform_int(int a = 0, int b) {
        //     std::uniform_int_distribution<int> dist(a, b);
        //     return dist(engine);
        // }

        // Normal distribution
        double normal(double mean = 0.0, double stddev = 1.0) {
            std::normal_distribution<double> dist(mean, stddev);
            return dist(engine);
        }

        // Exponential distribution
        double exponential(double lambda = 1.0) {
            std::exponential_distribution<double> dist(lambda);
            return dist(engine);
        }

        /// @brief A truncated normal randomly-sampled value. A truncated normal is the same as a normal distribution but with hard cutoffs that don't accumulate.
        /// @param mean The mean of the normal distribution from which to sample
        /// @param stddev The standard deviation of the normal distribution from which to sample
        /// @param min_val The minimum value of the distribution; sampled values can't be lower than this.
        /// @param max_val The maximum value of the distribution; sampled values can't be higher than this.
        /// @return A value sampled from a normal distribution of mean $\mu$, stddev $\sigma$, with bounds [min_val, max_val]
        double truncated_normal(double mean = 0.0, double stddev = 1.0, double min_val = -1.0, double max_val = 1.0) {
            return truncated_normal_icdf(this->engine, mean, stddev, min_val, max_val);
        }

    private:
        std::mt19937 engine;
    };
}
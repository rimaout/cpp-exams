#pragma once

#include <random>
#include <vector>
#include <stdexcept>

// Most of the code in this library was gaved me by [@aglaia](https://github.com/AglaiaNorza), 
// who modified a lib of [@ionutCicioiu](https://github.com/IonutCicio/software-engineering).

/**
 * @class Random
 * @brief A high-performance, thread-safe (via thread_local) Singleton for random number generation.
 * * This class wraps the std::mt19937 (Mersenne Twister) engine. It uses std::seed_seq 
 * to ensure the engine's large internal state is initialized with high entropy 
 * from the hardware's std::random_device.
 */
class Random {

    /* * QUICK NOTES:
     * - Singleton: Instantiating random generators is costly, so we only do it once.
     * - As a singleton, the methods are static.
     * - You call them directly on the class (e.g., Random::getUniformInt) !! No instances needed.
     */

private:
    std::mt19937 engine;

   /**
    * @brief Creates a properly seeded Mersenne Twister engine.
    * @return engine A high-quality random engine.
    */
    Random() {
        // - Get a source of hardware randomness
        std::random_device device_randomness_source;

        // - Use 'seed_seq' to properly seed the heavy state of the Mersenne Twister.
        //   std::random_device gives us 1 integer, but mt19937 needs about 624 integers 
        //   of state. seed_seq mixes the entropy to fill that state better.
        std::seed_seq seed_sequence {
            device_randomness_source(), 
            device_randomness_source(), 
            device_randomness_source(), 
            device_randomness_source()
        };
        engine.seed(seed_sequence);
    }

    /**
     * @brief Accesses the internal static instance.
     * @return Reference to the Singleton instance.
     */
    static Random& instance() {
        thread_local static Random inst;
        return inst;
    }

public:
    // Prevent copying and assignment
    Random(const Random&) = delete;
    Random& operator=(const Random&) = delete;

    /**
     * @brief Direct access to the underlying Mersenne Twister engine.
     * @return A reference to the std::mt19937 engine.
     */
    static std::mt19937& getEngine() {
        return instance().engine;
    }

    /**
     * @brief Generates an integer from a uniform distribution.
     * @param min The lower bound (inclusive).
     * @param max The upper bound (inclusive).
     * @return An integer value in the range [min, max].
     */
    static int getUniformInt(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(getEngine());
    }

    /**
     * @brief Generates a floating-point number from a uniform distribution.
     * @param min The lower bound (inclusive).
     * @param max The upper bound (exclusive).
     * @return A double value in the range [min, max).
     */
    static double getUniformReal(double min, double max) {
        std::uniform_real_distribution<double> dist(min, max);
        return dist(getEngine());
    }

    /**
     * @brief Performs a Bernoulli trial (a "coin flip").
     * @param p The probability of returning true (0.0 to 1.0).
     * @return True with probability p, false otherwise.
     */
    static bool getBernoulli(double p) {
        std::bernoulli_distribution dist(p);
        return dist(getEngine());
    }

    /**
     * @brief Generates a number from a Normal (Gaussian) distribution.
     * @param mean The center of the distribution.
     * @param stddev The standard deviation (spread) of the distribution.
     * @return A double sampled from the normal distribution.
     */
    static double getNormal(double mean, double stddev) {
        std::normal_distribution<double> dist(mean, stddev);
        return dist(getEngine());
    }

    /**
     * @brief Generates a value from an Exponential distribution.
     * @param lambda The rate parameter (often 1/mean).
     * @return A double representing the time between events in a Poisson process.
     */
    static double getExponential(double lambda) {
        std::exponential_distribution<double> dist(lambda);
        return dist(getEngine());
    }

    /**
     * @brief Selects an index based on provided weights (Discrete distribution).
     * @param weights A vector of probabilities or weights.
     * @return The index (0 to N-1) of the chosen weight.
     */
    static size_t getDiscrete(const std::vector<double>& weights) {
        if (weights.empty()) {
            throw std::invalid_argument("Weights vector cannot be empty.");
        }

        std::discrete_distribution<size_t> dist(weights.begin(), weights.end());
        return dist(getEngine());
    }
};
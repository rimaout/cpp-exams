#pragma once

#include <cmath>
#include <stdexcept>

// Most of the code in this library was gaved me by [@aglaia](https://github.com/AglaiaNorza), 
// who modified a lib of [@ionutCicioiu](https://github.com/IonutCicio/software-engineering).


/**
 * @brief Computes running statistics (mean, variance, standard deviation, sum) in a single pass.
 * * This class uses Welford's online algorithm to dynamically accumulate 
 * statistics. This approach is highly numerically stable compared to the 
 * naive method of tracking the sum of values and sum of squares.
 *
 * * @note POPULATION vs. SAMPLE VARIANCE
 *
 * The choice depends entirely on whether your data is the whole picture or just a piece of it.
 * 
 * * - Population Variance (Divides by N): Use this when your data represents the 
 * *entire* population you care about. You aren't guessing; you have the absolute truth.
 * 
 * * - Sample Variance (Divides by N-1): Use this when your data is a *subset* * used to estimate a larger population. Dividing by N-1 (Bessel's correction) 
 * slightly inflates the result to correct for the underestimation that happens 
 * when you only take a sample.
 * 
 * 
 * * REAL-WORLD EXAMPLES:
 * 
 * * 1. Factory Tiles: 
 * 
 * - Population: A laser scanner measures the thickness of all 100,000 tiles produced today.
 * 
 * - Sample: A worker randomly pulls 50 tiles off the line and measures them with calipers.
 * 
 * 
 * * 2. Server Logs: 
 * 
 * - Population: You feed every single HTTP request from today into this class to report on today's traffic.
 * 
 * * 3. Markov Chain / Monte Carlo: 
 * 
 * - Sample: You run 1,000 random walks to estimate the average cost of an infinite number of possible walks.
 */
class AccumulatorStatistics {
private:
    long count = 0;       ///< The number of values processed
    double sum = 0.0;     ///< The running total sum of all values
    double mean = 0.0;    ///< The current running mean
    double M2 = 0.0;      ///< The sum of squares of differences from the current mean

public:
    /**
     * @brief Adds a new value to the running statistics.
     * * @param value The data point to add.
     */
    void add(double value) {
        count++;
        sum += value;
        
        double delta = value - mean;
        mean += delta / count;
        double delta2 = value - mean;
        M2 += delta * delta2;
    }

    /**
     * @brief Retrieves the total sum of all added values.
     * * @return double The sum. Returns 0.0 if no values have been added.
     */
    double getTotalSum() const {
        return sum;
    }

    /**
     * @brief Retrieves the running mean (average).
     * * @return double The current mean. Returns 0.0 if no values have been added.
     */
    double getMean() const { 
        return mean; 
    }

    /**
     * @brief Retrieves the Sample Variance (uses Bessel's correction: N-1).
     * * Use this when your data is a sample of a larger population.
     * * @return double The sample variance. Returns 0.0 if fewer than 2 values exist.
     */
    double getSampleVariance() const {
        if (count < 2) return 0.0;
        return M2 / (count - 1);
    }

    /**
     * @brief Retrieves the Population Variance (uses N).
     * * Use this when your data represents the entire population.
     * * @return double The population variance. Returns 0.0 if fewer than 2 values exist.
     */
    double getPopulationVariance() const {
        if (count < 2) return 0.0;
        return M2 / count;
    }

    /**
     * @brief Retrieves the Sample Standard Deviation.
     * * @return double The sample standard deviation.
     */
    double getSampleStdDev() const {
        return std::sqrt(getSampleVariance());
    }

    /**
     * @brief Retrieves the Population Standard Deviation.
     * * @return double The population standard deviation.
     */
    double getPopulationStdDev() const {
        return std::sqrt(getPopulationVariance());
    }

    /**
     * @brief Retrieves the total number of values accumulated so far.
     * * @return long The count of data points.
     */
    long getCount() const { 
        return count; 
    } 

    /**
     * @brief Resets the accumulator to its initial state.
     */
    void reset() { 
        count = 0; 
        sum = 0.0;
        mean = 0.0; 
        M2 = 0.0; 
    }
};
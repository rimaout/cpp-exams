#pragma once

#include <random>

// Use mt19937 (Mersenne Twister 32-bit) instead of default_random_engine
using urng_t = std::mt19937;

/**
 * @brief Creates a properly seeded Mersenne Twister engine.
 * @return urng_t A high-quality random engine.
 */
inline urng_t pseudo_random_engine_from_device() {
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

    // - Initialize the engine with the sequence
    urng_t pseudo_random_engine(seed_sequence);

    return pseudo_random_engine;
}
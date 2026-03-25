#pragma once

/**
 * @brief General-purpose utilities.
 */
namespace utils {
    /**
     * @brief Generate random float in range [min, max).
     * @param min Range lower bound.
     * @param max Range upper bound.
     * @returns Random float.
     */
    float get_random_float(float min = 0.0f, float max = 1.0f);

    /**
     * @brief Generate random integer in range [min, max].
     * @param min Range lower bound.
     * @param max Range upper bound.
     * @returns Random integer.
     */
    int get_random_int(int min, int max);
}

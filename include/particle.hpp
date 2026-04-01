#pragma once

#include "raylib.h"

#include "config.hpp"
#include "theme.hpp"

#include <array>
#include <cstdint>
#include <vector>

/**
 * @brief Simulation particle management.
 */
namespace particle {
    /**
     * @brief Simulation particle.
     */
    struct alignas(8) Particle {
        Vector2 position;           ///< Current world position (px).
        Vector2 previous_position;  ///< Previous world position (px).
        uint32_t type;              ///< Particle type (zero-indexed).
        uint32_t padding;           ///< Padding to reach 24 bytes (compute shader).
    };

    /**
     * @brief Array of colors for each type of particle.
     */
    inline const std::array<Color, config::particle_types_max> colors = {
        theme::red, theme::green, theme::blue, theme::yellow,
        theme::purple, theme::aqua, theme::orange, theme::gray,
    };

    /**
     * @brief Initialize particle state.
     * @param particle Particle to initialize.
     * @param particle_types Current number of particle types.
     */
    void init(Particle& particle, int particle_types);

    /**
     * @brief Get color corresponding to particle type.
     * @param type Particle type.
     * @returns Associated color.
     */
    Color get_color(uint8_t type);

    /**
     * @brief Get attraction coefficient between two particle types.
     * @param type1 Type of particle one.
     * @param type2 Type of particle two.
     * @param matrix Attraction matrix.
     * @param particle_types Current number of particle types.
     * @returns Corresponding attraction coefficient.
     */
    float get_attraction_coefficient(uint8_t type1, uint8_t type2, const std::vector<float>& matrix, int particle_types);

    /**
     * @brief Set attraction coefficient between two particle types.
     * @param type1 Type of particle one.
     * @param type2 Type of particle two.
     * @param matrix Attraction matrix.
     * @param particle_types Current number of particle types.
     * @param coeff New coefficient (assumed to be valid).
     */
    void set_attraction_coefficient(uint8_t type1, uint8_t type2, std::vector<float>& matrix, int particle_types, float coeff);
}

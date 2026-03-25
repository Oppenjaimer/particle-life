#pragma once

#include "raylib.h"

#include "config.hpp"
#include "theme.hpp"

#include <array>
#include <cstdint>

/**
 * @brief Simulation particle management.
 */
namespace particle {
    /**
     * @brief Simulation particle.
     */
    struct Particle {
        uint8_t type;       ///< Particle type (zero-indexed).
        Vector2 position;   ///< Current world position (px).
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
     * @param camera Simulation camera.
     */
    void init(Particle& particle, int particle_types, Camera2D& camera);

    /**
     * @brief Update particle physics.
     * @param particle Particle to update.
     */
    void update(Particle& particle);

    /**
     * @brief Draw particle.
     * @param particle Particle to draw.
     */
    void draw(Particle& particle);
}

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
    struct Particle {
        uint8_t type;               ///< Particle type (zero-indexed).
        Vector2 position;           ///< Current world position (px).
        Vector2 previous_position;  ///< Previous world position (px).
        Vector2 acceleration;       ///< Current acceleration.
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
     * @brief Update all particles in the simulation.
     * @param particles Collection of particles.
     * @param matrix Attraction matrix.
     * @param friction Velocity damping factor.
     * @param dt Integration time step
     */
    void update(std::vector<Particle>& particles, const std::vector<std::vector<float>>& matrix, float friction, float dt);

    /**
     * @brief Draw particle.
     * @param particle Particle to draw.
     */
    void draw(const Particle& particle);
}

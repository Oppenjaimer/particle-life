#pragma once

#include "raylib.h"

#include "config.hpp"

/**
 * @brief Physics calculations for particle interactions.
 */
namespace physics {
    /**
     * @brief Runtime interaction settings.
     */
    struct InteractionCtx {
        float r_min = config::r_min;                ///< Initial minimum interaction distance between particles (px).
        float r_max = config::r_max;                ///< Initial maximum interaction distance between particles (px).
        float friction = config::friction;          ///< Velocity damping factor.
        float force_factor = config::force_factor;  ///< Force multiplier.
    };

    /**
     * @brief Calculate next position using damped Verlet integration.
     * @param pos Current position.
     * @param prev_pos Previous position.
     * @param acc Acceleration.
     * @param friction Velocity damping factor.
     * @param dt Time step.
     */
    void integrate_verlet(Vector2& pos, Vector2& prev_pos, Vector2& acc, float friction, float dt);

    /**
     * @brief Calculate interaction force between two particles.
     * @param a Attraction coefficient.
     * @param r Distance between the particles (px).
     * @returns Force magnitude.
     */
    float calculate_force(float a, float r);
}

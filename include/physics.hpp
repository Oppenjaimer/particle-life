#pragma once

#include "raylib.h"

#include "config.hpp"

/**
 * @brief Physics calculations for particle interactions.
 */
namespace physics {
    /**
     * @brief Type of world boundary.
     */
    enum BoundaryType {
        Bounce,     ///< Hard wall boundary condition, particles bounce off.
        Periodic,   ///< Periodic boundary condition, particles wrap around.
        Open,       ///< No boundary condition, particles fly off.
    };

    /**
     * @brief Runtime interaction settings.
     */
    struct InteractionCtx {
        float r_min = config::r_min;                ///< Initial minimum interaction distance between particles (px).
        float r_max = config::r_max;                ///< Initial maximum interaction distance between particles (px).
        float friction = config::friction;          ///< Velocity damping factor.
        float force_factor = config::force_factor;  ///< Force multiplier.

        BoundaryType boundary_type = Bounce;        ///< Type of world boundary.
    };

    /**
     * @brief Calculate next position using damped Verlet integration.
     * @param pos Current position.
     * @param prev_pos Previous position.
     * @param acc Acceleration.
     * @param dt Time step.
     * @param ctx Interaction context.
     */
    void integrate_verlet(Vector2& pos, Vector2& prev_pos, Vector2& acc, float dt, InteractionCtx& ctx);

    /**
     * @brief Calculate interaction force between two particles.
     * @param a Attraction coefficient.
     * @param r Distance between the particles (px).
     * @param ctx Interaction context.
     * @returns Force magnitude.
     */
    float calculate_force(float a, float r, InteractionCtx& ctx);
}

#include "physics.hpp"

#include "raymath.h" // Include after raylib.h

void physics::integrate_verlet(Vector2& pos, Vector2& prev_pos, Vector2& acc, float friction, float dt) {
    Vector2 velocity = Vector2Subtract(pos, prev_pos);
    Vector2 velocity_damped = Vector2Scale(velocity, 1.0f - friction);
    Vector2 acceleration = Vector2Scale(acc, dt * dt);

    prev_pos = pos;
    pos = Vector2Add(pos, velocity_damped);
    pos = Vector2Add(pos, acceleration);
    acc = {0.0f, 0.0f};
}

float physics::calculate_force(float a, float r) {
    if (r < config::r_min) {
        // Global repulsive force
        return r / config::r_min - 1.0f;
    } else if (r < config::r_max) {
        // Normal attractive/repulsive force
        return a * (1.0f - std::abs(2.0f * r - config::r_max - config::r_min) / (config::r_max - config::r_min));
    } else {
        // No force
        return 0.0f;
    }
}

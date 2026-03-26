#include "physics.hpp"

#include "raymath.h" // Include after raylib.h

void physics::integrate_verlet(Vector2& pos, Vector2& prev_pos, Vector2& acc, float friction, float dt) {
    Vector2 velocity = (pos - prev_pos) * (1.0f - friction);
    Vector2 acceleration = acc * dt * dt;

    prev_pos = pos;
    pos += velocity + acceleration;
    acc = {0.0f, 0.0f};
}

float physics::calculate_force(float a, float r, InteractionCtx& ctx) {
    if (r < ctx.r_min) {
        // Global repulsive force
        return r / ctx.r_min - 1.0f;
    } else if (r < ctx.r_max) {
        // Normal attractive/repulsive force
        return a * (1.0f - std::abs(2.0f * r - ctx.r_max - ctx.r_min) / (ctx.r_max - ctx.r_min));
    } else {
        // No force
        return 0.0f;
    }
}

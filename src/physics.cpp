#include "physics.hpp"

#include "raymath.h" // Include after raylib.h

void physics::integrate_verlet(Vector2& pos, Vector2& prev_pos, Vector2& acc, float dt, InteractionCtx& ctx) {
    Vector2 velocity = (pos - prev_pos) * (1.0f - ctx.friction);
    Vector2 acceleration = acc * dt * dt;

    prev_pos = pos;
    pos += velocity + acceleration;
    acc = {0.0f, 0.0f};

    // Apply boundary condition
    switch (ctx.boundary_type) {
        case BoundaryType::Bounce:
            if (pos.x < 0.0f) {
                pos.x = 0.0f;
                prev_pos.x = pos.x + velocity.x;
            } else if (pos.x > config::world_width) {
                pos.x = config::world_width;
                prev_pos.x = pos.x + velocity.x;
            }
            if (pos.y < 0.0f) {
                pos.y = 0.0f;
                prev_pos.y = pos.y + velocity.y;
            } else if (pos.y > config::world_height) {
                pos.y = config::world_height;
                prev_pos.y = pos.y + velocity.y;
            }
            break;

        case BoundaryType::Periodic:
            if (pos.x < 0.0f) {
                pos.x += config::world_width;
                prev_pos.x += config::world_width;
            } else if (pos.x >= config::world_width) {
                pos.x -= config::world_width;
                prev_pos.x -= config::world_width;
            }
            if (pos.y < 0.0f) {
                pos.y += config::world_height;
                prev_pos.y += config::world_height;
            } else if (pos.y >= config::world_height) {
                pos.y -= config::world_height;
                prev_pos.y -= config::world_height;
            }
            break;

        case BoundaryType::Open:
            break;
    }
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

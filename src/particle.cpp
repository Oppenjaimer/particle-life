#include "particle.hpp"
#include "utils.hpp"

#include "raymath.h" // Include after raylib.h

using namespace particle;

/**
 * @brief Get color corresponding to particle type.
 * @param type Particle type.
 * @returns Associated color.
 */
static Color get_color(uint8_t type) {
    return colors[type];
}

/**
 * @brief Calculate next position using damped Verlet integration.
 * @param pos Current position.
 * @param prev_pos Previous position.
 * @param acc Acceleration.
 * @param dt Time step.
 * @param friction Velocity damping factor.
 */
static void integrate_verlet(Vector2& pos, Vector2& prev_pos, Vector2& acc, float dt, float friction) {
    Vector2 velocity = Vector2Subtract(pos, prev_pos);
    Vector2 velocity_damped = Vector2Scale(velocity, 1.0f - friction);
    Vector2 acceleration = Vector2Scale(acc, dt * dt);

    prev_pos = pos;
    pos = Vector2Add(pos, velocity_damped);
    pos = Vector2Add(pos, acceleration);
    acc = {0.0f, 0.0f};
}

/**
 * @brief Calculate interaction force between two particles.
 * @param a Attraction coefficient.
 * @param r Distance between the particles (px).
 * @returns Force magnitude.
 */
static float calculate_force(float a, float r) {
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

void particle::init(Particle& particle, int particle_types, Camera2D& camera) {
    // Assign random type and position
    particle.type = utils::get_random_int(config::particle_types_min, particle_types) - 1;
    particle.position = GetScreenToWorld2D({
        utils::get_random_float(0.0f, GetScreenWidth()),
        utils::get_random_float(0.0f, GetScreenHeight())
    }, camera);

    // No acceleration or initial velocity
    particle.previous_position = particle.position;
    particle.acceleration = {0.0f, 0.0f};
}

void particle::update(std::vector<Particle> &particles, const std::vector<std::vector<float>>& matrix, float friction, float dt) {
    // First pass: calculate all forces based on current positions
    for (auto& particle : particles) {
        Vector2 total_force = {0.0f, 0.0f};

        for (const auto& other : particles) {
            if (&particle == &other) continue;

            Vector2 diff = Vector2Subtract(particle.position, other.position);
            float r = Vector2Length(diff);

            if (r > 0.0f && r < config::r_max) {
                float a = matrix[particle.type][other.type];
                float force = calculate_force(a, r) * config::force_factor;
                Vector2 direction = Vector2Normalize(diff);
                Vector2 force_vector = Vector2Scale(direction, -force);

                total_force = Vector2Add(total_force, force_vector);
            }
        }

        // Update acceleration (equal to force if mass is 1)
        particle.acceleration = total_force;
    }

    // Second pass: integrate over all particles
    for (auto& particle : particles) {
        integrate_verlet(particle.position, particle.previous_position, particle.acceleration, dt, friction);
    }
}

void particle::draw(const Particle& particle) {
    DrawCircleV(particle.position, config::particle_radius, get_color(particle.type));
}

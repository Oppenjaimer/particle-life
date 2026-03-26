#include "particle.hpp"
#include "physics.hpp"
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
                float force = physics::calculate_force(a, r) * config::force_factor;
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
        physics::integrate_verlet(particle.position, particle.previous_position, particle.acceleration, friction, dt);
    }
}

void particle::draw(const Particle& particle) {
    DrawCircleV(particle.position, config::particle_radius, get_color(particle.type));
}

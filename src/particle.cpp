#include "particle.hpp"
#include "utils.hpp"

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
    // Assign random values
    particle.type = utils::get_random_int(config::particle_types_min, particle_types) - 1;
    particle.position = GetScreenToWorld2D({
        utils::get_random_float(0.0f, GetScreenWidth()),
        utils::get_random_float(0.0f, GetScreenHeight())
    }, camera);
}

void particle::update(Particle& particle) {
    (void)particle;
}

void particle::draw(Particle& particle) {
    DrawCircleV(particle.position, config::particle_radius, get_color(particle.type));
}

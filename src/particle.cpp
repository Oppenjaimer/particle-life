#include "particle.hpp"
#include "utils.hpp"

void particle::init(Particle& particle, int particle_types) {
    // Assign random type and position
    particle.type = utils::get_random_int(config::particle_types_min, particle_types) - 1;
    particle.position = {
        utils::get_random_float(0.0f, config::world_width),
        utils::get_random_float(0.0f, config::world_height)
    };

    // No initial velocity
    particle.previous_position = particle.position;
}

Color particle::get_color(uint8_t type) {
    return colors[type];
}

float particle::get_attraction_coefficient(uint8_t type1, uint8_t type2, const std::vector<float>& matrix, int particle_types) {
    return matrix[type1 * particle_types + type2];
}

void particle::set_attraction_coefficient(uint8_t type1, uint8_t type2, std::vector<float>& matrix, int particle_types, float coeff) {
    matrix[type1 * particle_types + type2] = coeff;
}

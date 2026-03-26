#include "simulation.hpp"

using namespace sim;

/**
 * @brief Application entry point.
 * @returns Exit code.
 */
int main() {
    // Initialize global simulation state.
    State state;

    // Execute simulation lifecycle
    init(state);
    run(state);
    cleanup(state);

    return 0;
}

/*
 TODO:
 - Use 1D vector for matrix
 - Implement initial world grid (2500x2500) to spawn particles in
 - Add sliders for r_min, r_max, friction, force_factor (real-time, no need to pause)
 - Add matrix settings
 - Implement space partitioning and/or GPU rendering
 - Simulate only visible particles
 - Implement saving and loading settings (seeds)
 */

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
 - Make app logo
 - Implement space partitioning and/or GPU rendering
 - Simulate only visible particles
 - Implement saving and loading settings (seeds)
 - Add brush to create custom particle distributions
 - Add multiple themes
 */

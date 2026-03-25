#pragma once

#include "raylib.h"

#include "config.hpp"

/**
 * @brief Global simulation logic and state management.
 */
namespace sim {
    /**
     * @brief Simulation runtime state.
     */
    struct State {
        bool is_running = true;                             ///< Whether the main loop is active.
        bool is_paused = false;                             ///< Whether the simulation is paused.

        bool settings_collapse = config::settings_collapse; ///< Whether the settings panel is collapsed.
        bool settings_trigger = false;                      ///< Whether a settings panel collapse was triggered.

        Camera2D camera;                                    ///< Simulation 2D camera.
    };

    /**
     * @brief Initialize the simulation, window and GUI context.
     * @param state Simulation state to initialize.
     */
    void init(State& state);

    /**
     * @brief Reset simulation to its default state.
     * @param state Simulation state to reset.
     */
    void reset(State& state);

    /**
     * @brief Run main simulation loop. Blocking call until the window closes.
     * @param state Simulation state to run.
     */
    void run(State& state);

    /**
     * @brief Clean up resources and exit.
     * @param state Simulation state to clean up.
     */
    void cleanup(State& state);
}

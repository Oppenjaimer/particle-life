#pragma once

/**
 * @brief Global configuration constants.
 */
namespace config {
    // Application
    constexpr int fps = 60;                             ///< Target frames per second.
    constexpr const char* title = "Particle Life";      ///< Application window title.
    constexpr const char* resource_dir = "resources";   ///< Raylib resource directory name.

    // Screen
    constexpr int screen_width = 1280;                  ///< Initial screen width (px).
    constexpr int screen_height = 720;                  ///< Initial screen height (px).

    // World
    constexpr int world_width = 3000;                   ///< Width of the simulation world (px).
    constexpr int world_height = 3000;                  ///< Height of the simulation world (px).
    constexpr bool world_boundary = false;              ///< Whether to outline the world boundary.

    // Settings panel
    constexpr int settings_collapse = false;            ///< Default state of the settings panel.
    constexpr int settings_pos_x = 10;                  ///< Default X position of the settings panel.
    constexpr int settings_pos_y = 10;                  ///< Default Y position of the settings panel.
    constexpr int settings_width = 360;                 ///< Default width of the settings panel.
    constexpr float input_float_step = 1.0f;            ///< InputFloat step.
    constexpr float input_float_step_fast = 10.0f;      ///< InputFloat fast step.
    constexpr float matrix_cell_size = 30.0f;           ///< Attraction matrix square cell size.
    constexpr float matrix_circle_factor = 0.45f;       ///< Factor multiplying the cell size to get the color indicator radius.
    constexpr float matrix_wheel_sensitivity = 0.1f;    ///< Wheel sensitivity for attraction matrix updating.

    // Camera
    constexpr float pan_sensitivity_mouse = 1.0f;       ///< Drag sensitivity for mouse panning.
    constexpr float pan_sensitivity_keyboard = 10.0f;   ///< Key press sensitivity for keyboard panning.
    constexpr float zoom_sensitivity_mouse = 0.15f;     ///< Wheel sensitivity for mouse zooming.
    constexpr float zoom_sensitivity_keyboard = 0.08f;  ///< Key press sensitivity for keyboard zooming.
    constexpr float zoom_min = 0.1f;                    ///< Minimum camera zoom level.
    constexpr float zoom_max = 10.0f;                   ///< Maximum camera zoom level.

    // Particles
    constexpr int particle_count = 700;                 ///< Initial number of particles.
    constexpr int particle_count_min = 0;               ///< Minium number of particles.
    constexpr int particle_count_max = 100000;          ///< Maximum number of particles.
    constexpr int particle_types = 2;                   ///< Initial number of distinct particle types.
    constexpr int particle_types_min = 1;               ///< Minimum number of distinct particle types.
    constexpr int particle_types_max = 8;               ///< Maximum number of distinct particle types.
    constexpr float particle_radius = 3.0f;             ///< Particle drawing radius (px).

    // Interaction
    constexpr float r_min = 30.0f;                      ///< Minimum interaction distance between particles (px).
    constexpr float r_max = 200.0f;                     ///< Maximum interaction distance between particles (px).
    constexpr float friction = 0.2f;                    ///< Velocity damping factor.
    constexpr float force_factor = 1000.0f;             ///< Interaction force multiplier.
    constexpr float force_factor_max = 10000.0f;        ///< Interaction force multiplier.

    // Shaders
    constexpr int compute_groups = 256;                 ///< Number of workgroups in the compute shader.
}

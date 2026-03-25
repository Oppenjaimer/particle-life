#pragma once

/**
 * @brief Global configuration constants.
 */
namespace config {
    // Window
    constexpr int width = 1280;                         ///< Initial window width (px).
    constexpr int height = 720;                         ///< Initial window height (px).
    constexpr int fps = 60;                             ///< Target frames per second.

    // Settings panel
    constexpr int settings_collapse = false;            ///< Default state of the settings panel.
    constexpr int settings_pos_x = 10;                  ///< Default X position of the settings panel.
    constexpr int settings_pos_y = 10;                  ///< Default Y position of the settings panel.

    // Camera
    constexpr float pan_sensitivity_mouse = 1.0f;       ///< Drag sensitivity for mouse panning.
    constexpr float pan_sensitivity_keyboard = 10.0f;   ///< Key press sensitivity for keyboard panning.
    constexpr float zoom_sensitivity_mouse = 0.15f;     ///< Wheel sensitivity for mouse zooming.
    constexpr float zoom_sensitivity_keyboard = 0.08f;  ///< Key press sensitivity for keyboard zooming.
    constexpr float zoom_min = 0.1f;                    ///< Minimum camera zoom level.
    constexpr float zoom_max = 10.0f;                   ///< Maximum camera zoom level.

    // Particles
    constexpr int particle_count = 200;                 ///< Initial number of particles.
    constexpr int particle_count_min = 0;               ///< Minium number of particles.
    constexpr int particle_count_max = 1000000;         ///< Maximum number of particles.
    constexpr int particle_types = 2;                   ///< Initial number of distinct particle types.
    constexpr int particle_types_min = 1;               ///< Minimum number of distinct particle types.
    constexpr int particle_types_max = 8;               ///< Maximum number of distinct particle types.
    constexpr float particle_radius = 3.0f;             ///< Particle drawing radius.
}

#include "imgui.h"
#include "rlImGui.h"
#include "raymath.h"

#include "simulation.hpp"
#include "theme.hpp"

#include <cmath>

using namespace sim;

/**
 * @brief Handle camera panning with specific delta vector.
 * @param camera Camera to modify.
 * @param delta Movement vector.
 * @param mouse Whether the input is from a mouse or keyboard.
 */
static void pan(Camera2D& camera, Vector2 delta, bool mouse) {
    float sensitivity = mouse ? config::pan_sensitivity_mouse : config::pan_sensitivity_keyboard;
    delta = Vector2Scale(delta, -sensitivity / camera.zoom);
    camera.target = Vector2Add(camera.target, delta);
}

/**
 * @brief Handle camera panning via left mouse click and drag.
 * @param camera Camera to modify.
 */
static void pan(Camera2D& camera) {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        pan(camera, GetMouseDelta(), true);
}

/**
 * @brief Handle camera zooming with logarithmic scale.
 * @param camera Camera to modify.
 * @param value Zoom direction and amount.
 * @param mouse Whether the input is from a mouse or keyboard.
 */
static void zoom(Camera2D& camera, float value, bool mouse) {
    float sensitivity = mouse ? config::zoom_sensitivity_mouse : config::zoom_sensitivity_keyboard;
    float scale = sensitivity * value;
    camera.zoom = Clamp(expf(logf(camera.zoom) + scale), config::zoom_min, config::zoom_max);
}

/**
 * @brief Handle camera zooming via mouse wheel.
 * @param camera Camera to modify.
 */
static void zoom(Camera2D& camera) {
    float wheel = GetMouseWheelMove();
    if (wheel == 0) return;

    // Make cursor point match in screen space and world space
    Vector2 mouse_world = GetScreenToWorld2D(GetMousePosition(), camera);
    camera.offset = GetMousePosition();
    camera.target = mouse_world;

    zoom(camera, wheel, true);
}

/**
 * @brief Handle all mouse and keyboard input.
 * @param state Current simulation state.
 */
static void input(State& state) {
    ImGuiIO& io = ImGui::GetIO();

    // Quit simulation
    if (IsKeyPressed(KEY_Q))
        state.is_running = false;

    // Reset simulation
    if (IsKeyPressed(KEY_R))
        reset(state);

    // Toggle pause
    if (IsKeyPressed(KEY_SPACE))
        state.is_paused = !state.is_paused;

    // Toggle settings panel
    if (IsKeyPressed(KEY_S)) {
        state.settings_collapse = !state.settings_collapse;
        state.settings_trigger = true;
    }

    // Window resize
    if (IsWindowResized())
        state.camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};

    // Mouse camera controls (only when not in UI)
    if (!io.WantCaptureMouse) {
        pan(state.camera);
        zoom(state.camera);
    }

    // Keyboard camera controls (only when not in UI)
    if (!io.WantCaptureKeyboard) {
        // Pan
        Vector2 delta = {0, 0};
        if (IsKeyDown(KEY_UP)) delta.y = 1;
        if (IsKeyDown(KEY_DOWN)) delta.y = -1;
        if (IsKeyDown(KEY_LEFT)) delta.x = 1;
        if (IsKeyDown(KEY_RIGHT)) delta.x = -1;
        pan(state.camera, delta, false);

        // Zoom
        Vector2 center_screen = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
        Vector2 center_world = GetScreenToWorld2D(center_screen, state.camera);
        state.camera.offset = center_screen;
        state.camera.target = center_world;
        float value = 0.0f;
        if (IsKeyDown(KEY_SLASH)) value = -1.0f;
        if (IsKeyDown(KEY_RIGHT_BRACKET)) value = 1.0f;
        zoom(state.camera, value, false);
    }
}

/**
 * @brief Update simulation logic.
 * @param state Current simulation state.
 */
static void update(State& state) {
    if (state.is_paused) return;
}

/**
 * @brief Draw simulation components.
 * @param state Current simulation state.
 */
static void draw(State& state) {
    (void)state;

    DrawCircle(0, 0, 10, theme::orange);
}

/**
 * @brief Render ImGui interface.
 * @param state Current simulation state.
 */
static void gui(State& state) {
    // Settings panel collapse
    if (state.settings_trigger) {
        ImGui::SetNextWindowCollapsed(state.settings_collapse);
        state.settings_trigger = false;
    }

    // Settings panel initialization
    ImGui::Begin("Simulation Settings [S]", NULL, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SetWindowPos(ImVec2(config::settings_pos_x, config::settings_pos_y), ImGuiCond_FirstUseEver);
    ImGui::SetWindowCollapsed(state.settings_collapse, ImGuiCond_FirstUseEver);
    state.settings_collapse = ImGui::IsWindowCollapsed();

    // FPS counter
    ImGui::Text("FPS: %d", GetFPS());

    // Control buttons
    ImGui::SeparatorText("Controls");

    // Quit button
    if (ImGui::Button("Quit [Q]"))
        state.is_running = false;

    // Reset button
    ImGui::SameLine();
    if (ImGui::Button("Reset [R]"))
        reset(state);

    // Play/Pause button
    ImGui::SameLine();
    if (ImGui::Button(state.is_paused ? "Play [SPACE]" : "Pause [SPACE]"))
        state.is_paused = !state.is_paused;

    ImGui::End();
}

void sim::init(State& state) {
    // Initialize raylib
    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(config::width, config::height, "Simulation");
    SetTargetFPS(config::fps);

    // Initialize ImGui
    rlImGuiSetup(true);
    theme::apply();

    // Disable imgui.ini
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;

    // Initialize state
    reset(state);
}

void sim::reset(State& state) {
    // Reset camera
    state.camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    state.camera.target = {0.0f, 0.0f};
    state.camera.rotation = 0.0f;
    state.camera.zoom = 1.0f;
}

void sim::run(State& state) {
    while (state.is_running && !WindowShouldClose()) {
        // Logic
        input(state);
        update(state);

        BeginDrawing();
        ClearBackground(theme::bg0);

        // Draw
        BeginMode2D(state.camera);
        draw(state);
        EndMode2D();

        // GUI
        rlImGuiBegin();
        gui(state);
        rlImGuiEnd();

        EndDrawing();
    }
}

void sim::cleanup(State& state) {
    (void)state;

    rlImGuiShutdown();
    CloseWindow();
}

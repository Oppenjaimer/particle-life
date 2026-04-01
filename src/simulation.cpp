#include "imgui.h"
#include "rlImGui.h"
#include "raymath.h"
#include "rlgl.h"

#include "simulation.hpp"
#include "theme.hpp"
#include "utils.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>

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
 * @brief Toggle paused state and reset on play if settings were changed.
 * @param state Simulation state.
 */
static void toggle_pause(State& state) {
    if (state.is_paused) {
        if (state.particle_count != (int)state.particles.size() || state.particle_types != state.active_particle_types)
            reset(state);
    }

    state.is_paused = !state.is_paused;
}

/**
 * @brief Center the camera within the world.
 * @param camera Camera to center.
 */
static void center_camera(Camera2D& camera) {
    camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    camera.target = {config::world_width / 2.0f, config::world_height / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
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
        toggle_pause(state);

    // Center camera
    if (IsKeyPressed(KEY_C))
        center_camera(state.camera);

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

    rlEnableShader(state.compute_program);

    // Bind SSBOs to layout locations
    rlBindShaderBuffer(state.ssbo_particles, 0);
    rlBindShaderBuffer(state.ssbo_matrix, 1);

    // Send uniforms
    int world_width_loc = rlGetLocationUniform(state.compute_program, "world_width");
    rlSetUniform(world_width_loc, &config::world_width, RL_SHADER_UNIFORM_INT, 1);

    int world_height_loc = rlGetLocationUniform(state.compute_program, "world_height");
    rlSetUniform(world_height_loc, &config::world_height, RL_SHADER_UNIFORM_INT, 1);

    int count_loc = rlGetLocationUniform(state.compute_program, "particle_count");
    rlSetUniform(count_loc, &state.particle_count, RL_SHADER_UNIFORM_INT, 1);

    int types_loc = rlGetLocationUniform(state.compute_program, "particle_types");
    rlSetUniform(types_loc, &state.particle_types, RL_SHADER_UNIFORM_INT, 1);

    float dt = GetFrameTime();
    int dt_loc = rlGetLocationUniform(state.compute_program, "dt");
    rlSetUniform(dt_loc, &dt, RL_SHADER_UNIFORM_FLOAT, 1);

    int r_min_loc = rlGetLocationUniform(state.compute_program, "r_min");
    rlSetUniform(r_min_loc, &state.r_min, RL_SHADER_UNIFORM_FLOAT, 1);

    int r_max_loc = rlGetLocationUniform(state.compute_program, "r_max");
    rlSetUniform(r_max_loc, &state.r_max, RL_SHADER_UNIFORM_FLOAT, 1);

    int friction_loc = rlGetLocationUniform(state.compute_program, "friction");
    rlSetUniform(friction_loc, &state.friction, RL_SHADER_UNIFORM_FLOAT, 1);

    int force_factor_loc = rlGetLocationUniform(state.compute_program, "force_factor");
    rlSetUniform(force_factor_loc, &state.force_factor, RL_SHADER_UNIFORM_FLOAT, 1);

    int boundary = static_cast<int>(state.boundary_type);
    int boundary_type_loc = rlGetLocationUniform(state.compute_program, "boundary_type");
    rlSetUniform(boundary_type_loc, &boundary, RL_SHADER_UNIFORM_INT, 1);

    // Dispatch shader
    int num_groups = (state.particle_count + config::compute_groups - 1) / config::compute_groups;
    rlComputeShaderDispatch(num_groups, 1, 1);

    rlDisableShader();
}

/**
 * @brief Draw simulation components.
 * @param state Current simulation state.
 */
static void draw(const State& state) {
    // Draw world boundary
    if (state.world_boundary)
        DrawRectangleLines(0, 0, config::world_width, config::world_height, theme::bg4);

    BeginShaderMode(state.render_shader);

    // Set up camera perspective matrix
    Matrix mvp = MatrixMultiply(rlGetMatrixModelview(), rlGetMatrixProjection());
    int mvp_loc = GetShaderLocation(state.render_shader, "mvp");
    SetShaderValueMatrix(state.render_shader, mvp_loc, mvp);

    rlBindShaderBuffer(state.ssbo_particles, 0);

    // Update radius uniform
    int radius_loc = GetShaderLocation(state.render_shader, "particle_radius");
    float radius = config::particle_radius;
    SetShaderValue(state.render_shader, radius_loc, &radius, SHADER_UNIFORM_FLOAT);

    // Instanced draw
    rlEnableVertexArray(state.mesh.vaoId);
    rlDrawVertexArrayElementsInstanced(0, state.mesh.triangleCount * 3, NULL, state.particle_count);
    rlDisableVertexArray();

    EndShaderMode();
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

    // Settings panel fixed width
    ImGui::SetNextWindowSizeConstraints(
        ImVec2(config::settings_width, 0.0f),
        ImVec2(config::settings_width, FLT_MAX)
    );

    // Settings panel initialization
    ImGui::Begin("Simulation Settings [S]", NULL, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SetWindowPos(ImVec2(config::settings_pos_x, config::settings_pos_y), ImGuiCond_FirstUseEver);
    ImGui::SetWindowCollapsed(state.settings_collapse, ImGuiCond_FirstUseEver);
    state.settings_collapse = ImGui::IsWindowCollapsed();

    // FPS counter
    ImGui::Text("FPS: %d", GetFPS());

    ImGui::SeparatorText("World");

    // Show world boundary
    ImGui::AlignTextToFramePadding();
    ImGui::Text("World boundary                               "); ImGui::SameLine();
    ImGui::Checkbox("##show_boundary", &state.world_boundary);

    // World boundary type
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Boundary type     "); ImGui::SameLine();
    const char* boundary_names[] = {"Bounce", "Periodic", "Open"};
    int boundary_type = state.boundary_type;
    if (ImGui::Combo("##boundary_type", &boundary_type, boundary_names, IM_ARRAYSIZE(boundary_names)))
        state.boundary_type = static_cast<BoundaryType>(boundary_type);

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
        toggle_pause(state);

    // Center camera button
    if (ImGui::Button("Center camera [C]"))
        center_camera(state.camera);

    ImGui::SeparatorText("Particles");
    ImGui::BeginDisabled(!state.is_paused);

    // Particle count
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Count             "); ImGui::SameLine();
    if (ImGui::InputInt("##count", &state.particle_count))
        state.particle_count = std::clamp(state.particle_count, config::particle_count_min, config::particle_count_max);

    // Particle types
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Types             "); ImGui::SameLine();
    if (ImGui::InputInt("##types", &state.particle_types))
        state.particle_types = std::clamp(state.particle_types, config::particle_types_min, config::particle_types_max);

    ImGui::EndDisabled();

    ImGui::SeparatorText("Interaction");

    // Minimum distance
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Minimum distance  "); ImGui::SameLine();
    if (ImGui::InputFloat("##r_min", &state.r_min, config::input_float_step, config::input_float_step_fast))
        state.r_min = std::max(0.0f, state.r_min);

    // Maximum distance
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Maximum distance  "); ImGui::SameLine();
    if (ImGui::InputFloat("##r_max", &state.r_max, config::input_float_step, config::input_float_step_fast))
        state.r_max = std::max(0.0f, state.r_max);

    // Friction
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Friction          "); ImGui::SameLine();
    ImGui::SliderFloat("##friction", &state.friction, 0.0f, 1.0f);

    // Force factor
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Force factor      "); ImGui::SameLine();
    ImGui::SliderFloat("##force_factor", &state.force_factor, 0.0f, config::force_factor_max);

    ImGui::SeparatorText("Attraction Matrix");

    // Attraction matrix
    if (ImGui::BeginTable("matrix", state.active_particle_types + 1, ImGuiTableFlags_SizingFixedFit)) {
        for (int row = 0; row < state.active_particle_types + 1; row++) {
            ImGui::TableNextRow();

            for (int col = 0; col < state.active_particle_types + 1; col++) {
                ImGui::TableSetColumnIndex(col);

                if (row == 0 && col == 0) {
                    // Skip top-left cell
                    continue;
                } else if (row == 0 || col == 0) {
                    // Color indicators
                    int color_index = (row == 0) ? (col - 1) : (row - 1);
                    Color color = particle::get_color(color_index);
                    ImU32 fg = ImGui::GetColorU32(theme::to_imvec(color));

                    ImVec2 min_pos = ImGui::GetCursorScreenPos();
                    ImVec2 center = ImVec2(min_pos.x + config::matrix_cell_size * 0.5f, min_pos.y + config::matrix_cell_size * 0.5f);

                    ImDrawList* draw_list = ImGui::GetWindowDrawList();
                    float radius = config::matrix_cell_size * config::matrix_circle_factor;

                    ImGui::Dummy(ImVec2(config::matrix_cell_size, config::matrix_cell_size));
                    draw_list->AddCircleFilled(center, radius, fg);
                } else {
                    // Matrix coefficients
                    float coeff = particle::get_attraction_coefficient(row - 1, col - 1, state.matrix, state.active_particle_types);
                    Color color = (coeff < 0) ? theme::bright_red : theme::bright_green;
                    color.a = std::abs(coeff) * 255.0f;
                    ImU32 bg = ImGui::GetColorU32(theme::to_imvec(color));

                    ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, bg);
                    ImGui::Dummy(ImVec2(config::matrix_cell_size, config::matrix_cell_size));

                    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNone)) {
                        // Change attraction coefficient value with mouse wheel
                        float wheel = ImGui::GetIO().MouseWheel;
                        if (wheel != 0.0f) {
                            float new_coeff = std::clamp(coeff + wheel * config::matrix_wheel_sensitivity, -1.0f, 1.0f);
                            particle::set_attraction_coefficient(row - 1, col - 1, state.matrix, state.active_particle_types, new_coeff);
                        }

                        // Display attraction coefficient in tooltip
                        ImGui::BeginTooltip();
                        ImGui::Text("%.3f", coeff);
                        ImGui::EndTooltip();
                    }
                }
            }
        }

        ImGui::EndTable();
    }

    ImGui::End();
}

/**
 * @brief Create a quad facing the camera.
 * @returns Quad mesh.
 */
static Mesh create_mesh() {
    Mesh mesh = {};
    mesh.vertexCount = 4;
    mesh.triangleCount = 2;

    mesh.vertices = (float*)MemAlloc(mesh.vertexCount * 3 * sizeof(float));
    mesh.texcoords = (float*)MemAlloc(mesh.vertexCount * 2 * sizeof(float));
    mesh.indices = (unsigned short*)MemAlloc(mesh.triangleCount * 3 * sizeof(unsigned short));

    // Top-left
    mesh.vertices[0] = -0.5f;
    mesh.vertices[1] = -0.5f;
    mesh.vertices[2] = 0.0f;
    mesh.texcoords[0] = 0.0f;
    mesh.texcoords[1] = 0.0f;
    // Bottom-left
    mesh.vertices[3] = -0.5f;
    mesh.vertices[4] = 0.5f;
    mesh.vertices[5] = 0.0f;
    mesh.texcoords[2] = 0.0f;
    mesh.texcoords[3] = 1.0f;
    // Bottom-right
    mesh.vertices[6] = 0.5f;
    mesh.vertices[7] = 0.5f;
    mesh.vertices[8] = 0.0f;
    mesh.texcoords[4] = 1.0f;
    mesh.texcoords[5] = 1.0f;
    // Top-right
    mesh.vertices[9] = 0.5f;
    mesh.vertices[10] = -0.5f;
    mesh.vertices[11] = 0.0f;
    mesh.texcoords[6] = 1.0f;
    mesh.texcoords[7] = 0.0f;

    // Indices
    mesh.indices[0] = 0;
    mesh.indices[1] = 1;
    mesh.indices[2] = 2;
    mesh.indices[3] = 0;
    mesh.indices[4] = 2;
    mesh.indices[5] = 3;

    UploadMesh(&mesh, false);
    return mesh;
}

void sim::init(State& state) {
    // Initialize raylib
    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(config::screen_width, config::screen_height, config::title);
    SetTargetFPS(config::fps);

    // Set resources directory and app icon
    bool found = utils::set_resource_dir(config::resource_dir);
    if (found) {
        Image icon = LoadImage("icon.png");
        SetWindowIcon(icon);
    } else {
        TraceLog(LOG_WARNING, "Unable to find '%s' directory", config::resource_dir);
    }

    // Initialize ImGui
    rlImGuiSetup(true);
    theme::apply();

    // Disable imgui.ini
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;

    // Load compute shader
    char* compute_code = LoadFileText("particle_update.comp");
    unsigned int compute_shader = rlLoadShader(compute_code, RL_COMPUTE_SHADER);
    state.compute_program = rlLoadShaderProgramCompute(compute_shader);
    UnloadFileText(compute_code);

    // Create mesh and render shader
    state.mesh = create_mesh();
    state.render_shader = LoadShader("particle_draw.vs", "particle_draw.fs");

    // Upload colors arrray to GPU
    int colors_loc = GetShaderLocation(state.render_shader, "colors");
    Vector4 color_data[config::particle_types_max];
    for (int i = 0; i < config::particle_types_max; i++) {
        Color c = particle::colors[i];
        color_data[i] = {c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, c.a / 255.0f};
    }
    SetShaderValueV(state.render_shader, colors_loc, color_data, SHADER_UNIFORM_VEC4, config::particle_types_max);

    // Initialize camera and state
    center_camera(state.camera);
    reset(state);
}

void sim::reset(State& state) {
    // Reset particles
    state.particles.clear();
    state.particles.resize(state.particle_count);

    // Initialize each particle
    for (auto& particle : state.particles) {
        particle::init(particle, state.particle_types);
    }

    // Update active particle types
    state.active_particle_types = state.particle_types;

    // Initialize matrix with random values
    state.matrix.resize(state.particle_types * state.particle_types);
    for (int i = 0; i < state.particle_types; i++) {
        for (int j = 0; j < state.particle_types; j++) {
            state.matrix[i * state.particle_types + j] = utils::get_random_float(-1.0f, 1.0f);
        }
    }

    // Clean up old SSBOs
    if (state.ssbo_particles != 0) rlUnloadShaderBuffer(state.ssbo_particles);
    if (state.ssbo_matrix != 0) rlUnloadShaderBuffer(state.ssbo_matrix);

    // Create SSBOs
    state.ssbo_particles = rlLoadShaderBuffer(
        state.particles.size() * sizeof(particle::Particle),
        state.particles.data(),
        RL_DYNAMIC_COPY
    );

    state.ssbo_matrix = rlLoadShaderBuffer(
        state.matrix.size() * sizeof(float),
        state.matrix.data(),
        RL_DYNAMIC_COPY
    );
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
    UnloadShader(state.render_shader);
    UnloadMesh(state.mesh);
    rlUnloadShaderProgram(state.compute_program);
    rlUnloadShaderBuffer(state.ssbo_particles);
    rlUnloadShaderBuffer(state.ssbo_matrix);
    rlImGuiShutdown();
    CloseWindow();
}

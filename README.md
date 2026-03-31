# Particle Life

A Raylib implementation of Particle Life in C++. The codebase is written in a minimalist, procedural style, treating C++ as a "better C." It relies on POD structs and namespace-scoped functions rather than heavy object-oriented paradigms. It is also documented with [Doxygen](https://www.doxygen.nl/manual/docblocks.html).

## Libraries

* [Raylib](https://www.raylib.com/): Compiled from source for OpenGL 4.3 support.
* [Dear ImGui](https://github.com/ocornut/imgui): Cloned to `external/imgui`.
* [rlImGui](https://github.com/raylib-extras/rlImGui): Cloned to `external/rlimgui`.

## Features

- [x] **Infinite** 2D scene with **panning** and **zooming**.
- [x] **Keyboard shortcuts** for controlling the simulation (**quit**, **reset**, **pause**).
- [x] Collapsible **settings panel** which can be moved around.
- [ ] **GPU rendering** for better performance.
- [x] Default **Gruvbox Dark** theme.

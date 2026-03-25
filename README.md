# Simulation Template

A Raylib + Dear ImGui simulation template for C++. The codebase is written in a minimalist, procedural style, treating C++ as a "better C." It relies on POD structs and namespace-scoped functions rather than heavy object-oriented paradigms. It is also documented with [Doxygen](https://www.doxygen.nl/manual/docblocks.html).

## Libraries

* [Raylib](https://www.raylib.com/): Installed via package manager ([Arch](https://archlinux.org/packages/extra/x86_64/raylib/)).
* [Dear ImGui](https://github.com/ocornut/imgui): Cloned to `external/imgui`.
* [rlImGui](https://github.com/raylib-extras/rlImGui): Cloned to `external/rlimgui`.

## Features

- [x] **Infinite** 2D scene with **panning** and **zooming**.
- [ ] **Dynamic grid** which automatically adjusts to the current zoom level.
- [x] **Keyboard shortcuts** for controlling the simulation (**quit**, **reset**, **pause**).
- [x] Collapsible **settings panel** which can be moved around.
- [ ] **GPU rendering** for better performance.
- [ ] **Operator overloading** for `Vector2`.
- [x] **Random float** and **integer** generation.
- [x] Default **Gruvbox Dark** theme.

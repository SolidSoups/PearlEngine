# PEARL ENGINE

This is the repository for the pearl engine. Currently unable to build the game seperately, but a fix is coming for it.

## Installation

**Dependencies** (pulled via git submodules):
- [GLFW](https://github.com/glfw/glfw) - window and input
- [GLM](https://github.com/g-truc/glm) - math
- [ImGui](https://github.com/ocornut/imgui) - UI (docking branch)
- [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo) - gizmos
- [nlohmann/json](https://github.com/nlohmann/json) - JSON serialization
- [sol2](https://github.com/ThePhD/sol2) - Lua bindings
- glad - OpenGL loader (vendored)
- Lua 5.4.7 - scripting (vendored)
- stb_image - texture loading (vendored)

**Requirements**:
- CMake 3.16
- C++17
- pkg-config (LINUX ONLY)
- GTK4 (LINUX ONLY)
- X11 (LINUX ONLY)

As you can see, there are 3 packages which are Linux only. These will be removed so that the project can be compiled cross platform.

**Install and Setup**:
1. Clone the repo with either git/github desktop
2. Navigate to the root of the repository
3. Pull all git submodules to get the dependencies. Use ```git submodule update --init --recursive```.
4. Compile with ```cmake -B build -S . -G Ninja```
5. Build with ```cmake --build build```
6. The executable can be run from *./build/game_engine*

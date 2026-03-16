# PEARL ENGINE

This is the repository for the pearl engine. Currently unable to build the game seperately, but a fix is coming for it.

## Installation

**Dependencies**:
- [GLFW](https://github.com/glfw/glfw) - window and input
- [GLM](https://github.com/g-truc/glm) - math
- [ImGui](https://github.com/ocornut/imgui) - UI (docking branch)
- [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo) - gizmos
- [nlohmann/json](https://github.com/nlohmann/json) - JSON serialization
- [sol2](https://github.com/ThePhD/sol2) - Lua bindings
- glad - OpenGL loader 
- Lua 5.4.7 - scripting 
- stb_image - texture loading 

The dependencies are pulled via git submodules.

**Requirements**:
- CMake 3.16, with Ninja generator
- C++20

**Install and Setup**:
1. Clone the repo with either git/github desktop
2. Navigate to the root of the repository
3. Pull all git submodules to get the dependencies. Use ```git submodule update --init --recursive```.

To compile the standalone game:
```
cd PearlEngine
mkdir build
cmake -B build -S . -G Ninja -DPEARL_EDITOR=OFF
ninja -C build
```

To compile with the editor attached:
```
cd PearlEngine
mkdir editor_build
cmake -B editor_build -S . -G Ninja -DPEARL_EDITOR=ON
ninja -C editor_build
```

## Content

- A controllable player character. Use WASD to move around, and space to jump (up to two times)
- A thirdperson camera, which automatically follows the player
- Three levels, with start, goal and end. Platforms.
- Interactable switch object (Level2)
- Record times system
- Custom menu for selecting levels and viewing times
- A couple enemies (Level2)
- Terrain mesh with physics interaction (Level3)
- Basic moving objects the player can interact with (Level1)
- Basic physical objects that are not the player (Level3)
- Custom text rendering
- View best times from the menu, saved to *my/LevelTimes.json*
- Able to pause game with ESC



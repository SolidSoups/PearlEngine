cmake_minimum_required(VERSION 3.16)
project(PearlEngine)

set(CMAKE_CXX_STANDARD 20)

option(PEARL_EDITOR "Build with the editor included" ON)

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-mavx2" COMPILER_SUPPORTS_AVX2)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")

# Enable colored compiler diagnostics
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-fdiagnostics-color=always)
endif()

# enable ccache
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
  set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

# Set default build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)

# gtk4 (for file-explorer) - DISABLED: not currently used
# find_package(PkgConfig REQUIRED)
# pkg_check_modules(GTK4 REQUIRED gtk4)

# glad library
add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

find_package(X11 REQUIRED)

# Lua
file(GLOB LUA_SOURCES "external/lua/*.c")
list(REMOVE_ITEM LUA_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/external/lua/lua.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/external/lua/luac.c"
)

add_library(lua STATIC ${LUA_SOURCES})
target_include_directories(lua PUBLIC external/lua)

# nlohmann/json
add_subdirectory(external/json)

# imgui library
add_library(imgui 
  external/imgui/imgui.cpp
  external/imgui/imgui_demo.cpp
  external/imgui/imgui_draw.cpp
  external/imgui/imgui_tables.cpp
  external/imgui/imgui_widgets.cpp
  external/imgui/backends/imgui_impl_glfw.cpp
  external/imgui/backends/imgui_impl_opengl3.cpp

  external/imguizmo/ImGuizmo.cpp
)
target_include_directories(imgui 
  PUBLIC
    external/imgui
    external/imgui/backends
    external/imguizmo
)
target_link_libraries(imgui
  PUBLIC 
    glfw
    glad
    nlohmann_json::nlohmann_json
    ${X11_LIBRARIES}
)

# Sol2 (header-only)
add_executable(game_engine)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
target_sources(game_engine PRIVATE 
  external/stb/stb_image.cpp
  ${SOURCES}
)
target_link_libraries(game_engine
  PRIVATE
    glfw
    glad
    lua
    imgui
    # ${GTK4_LIBRARIES}
)
target_include_directories(game_engine PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/struct
  ${CMAKE_CURRENT_SOURCE_DIR}/src/core
  ${CMAKE_CURRENT_SOURCE_DIR}/src/rendering
  ${CMAKE_CURRENT_SOURCE_DIR}/src/geometry
  ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
  ${CMAKE_CURRENT_SOURCE_DIR}/src/utils

  ${CMAKE_CURRENT_SOURCE_DIR}/src/editor
  ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/panels
  ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/popups
  ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/componenteditors

  ${CMAKE_CURRENT_SOURCE_DIR}/src/resourceNew

  ${CMAKE_CURRENT_SOURCE_DIR}/src/resource
  ${CMAKE_CURRENT_SOURCE_DIR}/src/resource/resources
  ${CMAKE_CURRENT_SOURCE_DIR}/src/resource/creation

  ${CMAKE_CURRENT_SOURCE_DIR}/src/project

  ${CMAKE_CURRENT_SOURCE_DIR}/src/physics

  ${CMAKE_CURRENT_SOURCE_DIR}/src/service

  ${CMAKE_CURRENT_SOURCE_DIR}/src/file

  ${CMAKE_CURRENT_SOURCE_DIR}/src/message

  ${CMAKE_CURRENT_SOURCE_DIR}/src/ecs
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ecs/components
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ecs/systems
  external/stb
  external/glm
  ${CMAKE_CURRENT_SOURCE_DIR}/external
  external/lua
  external/sol2/include
  ${CMAKE_CURRENT_SOURCE_DIR}/src/scripting
  # ${GTK4_INCLUDE_DIRS}
)
target_compile_definitions(game_engine PRIVATE
  DEBUG_ENABLED=1
  LOG_TO_CONSOLE=1
  SOL_NO_LUA_HPP=1   # Prevent sol2 from using /usr/local/include/lua.hpp (Lua 5.1)
  $<$<BOOL:${COMPILER_SUPPORTS_AVX2}>:PEARL_HAS_AVX2>
  $<$<BOOL:${PEARL_EDITOR}>:PEARL_EDITOR>
)
target_precompile_headers(game_engine PRIVATE
  # Glad MUST preced GLFW - pch preservse this order!!:W
  <glad/glad.h>
  <GLFW/glfw3.h>
  # imgui
  <imgui.h>
  # Math
  <glm/glm.hpp>
  <glm/gtc/matrix_transform.hpp>
  <glm/gtc/type_ptr.hpp>
  # libarries
  <stb_image.h>
  # standard libary
  <string>
  <vector>
  <memory>
  <unordered_map>
  <functional>
  <iostream>
  # my library
  src/utils/Logger.h
)
if(COMPILER_SUPPORTS_AVX2)
  target_compile_options(game_engine PRIVATE -mavx2)
endif()

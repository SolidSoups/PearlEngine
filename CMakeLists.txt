cmake_minimum_required(VERSION 3.16)
project(PearlEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable colored compiler diagnostics
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-fdiagnostics-color=always)
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

# gtk4 (for file-explorer)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

# glad library
add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

find_package(X11 REQUIRED)

# imgui library
add_library(imgui 
  external/imgui/imgui.cpp
  external/imgui/imgui_demo.cpp
  external/imgui/imgui_draw.cpp
  external/imgui/imgui_tables.cpp
  external/imgui/imgui_widgets.cpp
  external/imgui/backends/imgui_impl_glfw.cpp
  external/imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui 
  PUBLIC
    external/imgui
    external/imgui/backends
)
target_link_libraries(imgui
  PUBLIC 
    glfw
    glad
    ${X11_LIBRARIES}
)

# rapidjson
set(RAPIDJSON_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(external/rapidjson)

# target
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
    imgui
    RapidJSON
    ${GTK4_LIBRARIES}
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
  ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/componenteditors

  ${CMAKE_CURRENT_SOURCE_DIR}/src/resourceNew

  ${CMAKE_CURRENT_SOURCE_DIR}/src/resource
  ${CMAKE_CURRENT_SOURCE_DIR}/src/resource/loaders
  ${CMAKE_CURRENT_SOURCE_DIR}/src/resource/resources
  ${CMAKE_CURRENT_SOURCE_DIR}/src/resource/converters

  ${CMAKE_CURRENT_SOURCE_DIR}/src/project

  ${CMAKE_CURRENT_SOURCE_DIR}/src/service

  ${CMAKE_CURRENT_SOURCE_DIR}/src/file

  ${CMAKE_CURRENT_SOURCE_DIR}/src/asset
  ${CMAKE_CURRENT_SOURCE_DIR}/src/asset/loaders
  ${CMAKE_CURRENT_SOURCE_DIR}/src/asset/assets
  ${CMAKE_CURRENT_SOURCE_DIR}/src/asset/serializers
  ${CMAKE_CURRENT_SOURCE_DIR}/src/asset/converters

  ${CMAKE_CURRENT_SOURCE_DIR}/src/message

  ${CMAKE_CURRENT_SOURCE_DIR}/src/ecs
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ecs/components
  external/stb
  external/glm
  external/rapidjson/include
  ${GTK4_INCLUDE_DIRS}
)
target_compile_definitions(game_engine PRIVATE
  DEBUG_ENABLED=1 
  LOG_TO_CONSOLE=1
)

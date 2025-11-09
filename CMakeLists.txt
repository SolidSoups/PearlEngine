cmake_minimum_required(VERSION 3.16)
project(PearlEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)

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

# target
add_executable(game_engine)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
target_sources(game_engine PRIVATE ${SOURCES})
target_link_libraries(game_engine 
  PRIVATE 
    glfw 
    glad 
    imgui
)
target_include_directories(game_engine PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/core
  ${CMAKE_CURRENT_SOURCE_DIR}/src/rendering
  ${CMAKE_CURRENT_SOURCE_DIR}/src/geometry
  ${CMAKE_CURRENT_SOURCE_DIR}/src/components
  ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
  ${CMAKE_CURRENT_SOURCE_DIR}/src/editor
  external/glm
)

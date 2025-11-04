cmake_minimum_required(VERSION 3.16)
project(PearlEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)

add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

add_executable(game_engine)
target_sources(game_engine PRIVATE src/main.cpp)
target_link_libraries(game_engine PRIVATE glfw glad)
target_include_directories(game_engine PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

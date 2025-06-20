cmake_minimum_required(VERSION 3.10)
project(matfx)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

add_subdirectory(deps/glfw)

find_package(OpenGL REQUIRED)

add_executable(matfx src/main.cpp src/window.cpp src/renderer.cpp)
target_link_libraries(matfx glfw OpenGL::GL)
target_include_directories(matfx PRIVATE deps/glfw/include deps/sokol)
target_compile_definitions(matfx PRIVATE GLFW_INCLUDE_NONE SOKOL_GLCORE)

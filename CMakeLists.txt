cmake_minimum_required(VERSION 3.10)
project(matfx)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()

add_subdirectory(deps/glfw)

add_executable(matfx 
    src/main.cpp 
    src/window.cpp 
    src/renderer.cpp 
    deps/glad/src/glad.c
    deps/spdlog/src/bundled_fmtlib_format.cpp
    deps/spdlog/src/spdlog.cpp
    deps/spdlog/src/stdout_sinks.cpp
    deps/spdlog/src/color_sinks.cpp
    deps/spdlog/src/file_sinks.cpp
    deps/spdlog/src/async.cpp
    deps/spdlog/src/cfg.cpp
)
target_link_libraries(matfx glfw)
target_include_directories(matfx PRIVATE 
    deps/glfw/include 
    deps/sokol 
    deps/glm 
    deps/entt/src 
    deps/glad/include 
    deps/spdlog/include
)
target_compile_definitions(matfx PRIVATE 
    GLFW_INCLUDE_NONE 
    SOKOL_GLCORE 
    SPDLOG_COMPILED_LIB 
    $<$<CONFIG:Debug>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE> 
    $<$<CONFIG:Release>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO>
)

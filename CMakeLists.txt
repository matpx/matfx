cmake_minimum_required(VERSION 3.10)
project(matfx)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Check for AVX2 support
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
    set(ENABLE_AVX2 ON)
else()
    set(ENABLE_AVX2 OFF)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    if(ENABLE_AVX2)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2")
    endif()
endif()

add_subdirectory(deps/glfw)

set(SPDLOG_SOURCES
    deps/spdlog/src/bundled_fmtlib_format.cpp
    deps/spdlog/src/spdlog.cpp
    deps/spdlog/src/stdout_sinks.cpp
    deps/spdlog/src/color_sinks.cpp
    deps/spdlog/src/file_sinks.cpp
    deps/spdlog/src/async.cpp
    deps/spdlog/src/cfg.cpp
)

add_executable(matfx 
    src/main.cpp 
    src/window.cpp 
    src/renderer.cpp 
    deps/glad/src/glad.c
    $<$<CONFIG:Debug>:${SPDLOG_SOURCES}>
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
    $<$<CONFIG:Debug>:SPDLOG_COMPILED_LIB>
    $<$<CONFIG:Debug>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE> 
    $<$<CONFIG:Release>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO>
    $<$<BOOL:${ENABLE_AVX2}>:GLM_FORCE_AVX2>
    $<$<BOOL:${ENABLE_AVX2}>:GLM_FORCE_ALIGNED>
)

#include "renderer.h"
#include "spdlog/common.h"
#include "window.h"

#define SOKOL_IMPL
#define SOKOL_EXTERNAL_GL_LOADER
#include <glad/glad.h>
#include <sokol_gfx.h>
#include <sokol_log.h>
#include <spdlog/spdlog.h>

static bool run_engine() {
  SPDLOG_INFO("Starting MatFX engine");

  Window window(800, 600, "MatFX");
  if (!window.init()) {
    SPDLOG_ERROR("Failed to initialize window");
    return false;
  }
  SPDLOG_INFO("Window initialized successfully");

  Renderer renderer;
  if (!renderer.init()) {
    SPDLOG_ERROR("Failed to initialize renderer");
    return false;
  }
  SPDLOG_INFO("Renderer initialized successfully");

  SPDLOG_INFO("Entering main loop");
  while (!window.shouldClose()) {
    int width, height;
    window.getFramebufferSize(width, height);

    renderer.beginFrame(width, height);
    renderer.render();
    renderer.endFrame();

    window.swapBuffers();
    window.pollEvents();
  }

  SPDLOG_INFO("Main loop ended, shutting down");
  return true;
}

int main() {
#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_TRACE
  spdlog::set_level(spdlog::level::trace);
#else
  spdlog::set_level(spdlog::level::info);
#endif

  SPDLOG_INFO("MatFX application starting");

  try {
    if (!run_engine()) {
      SPDLOG_ERROR("Engine failed to run");
      return -1;
    }
  } catch (const std::exception &e) {
    SPDLOG_ERROR("Exception caught: {}", e.what());
    return -1;
  } catch (...) {
    SPDLOG_ERROR("Unknown exception caught");
    return -1;
  }

  SPDLOG_INFO("MatFX application exiting successfully");
  return 0;
}
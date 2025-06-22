#include <spdlog/spdlog.h>

#include "renderer.h"
#include "spdlog/common.h"
#include "window.h"

static bool run_engine() {
  SPDLOG_INFO("Starting MatFX engine");

  Window window(800, 600, "MatFX");
  if (!window.init()) {
    return false;
  }

  Renderer renderer;
  if (!renderer.init()) {
    return false;
  }

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

  spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%s:%#] %v");

  SPDLOG_INFO("MatFX application starting");
  SPDLOG_DEBUG("debug mode enabled");

#ifdef __AVX2__
  SPDLOG_INFO("AVX2 support enabled");
#else
  SPDLOG_WARN("AVX2 support not available");
#endif

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
#pragma once
#include <sokol_gfx.h>

class Renderer {
 public:
  Renderer();
  ~Renderer();

  Renderer(const Renderer &) = delete;
  Renderer &operator=(const Renderer &) = delete;
  Renderer(Renderer &&) = delete;
  Renderer &operator=(Renderer &&) = delete;

  bool init();
  void shutdown();

  void beginFrame(int width, int height);
  void render();
  void endFrame();

 private:
  sg_pipeline pipeline_;
  sg_buffer vertex_buffer_;
  bool initialized_;

  void setupTriangle();
};
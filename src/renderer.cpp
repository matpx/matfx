#include "renderer.h"

#include <glad/glad.h>
#include <sokol_log.h>
#include <spdlog/spdlog.h>

#include <array>

static void custom_sokol_logger(const char* tag, uint32_t log_level, uint32_t /* log_item_id */, const char* message,
                                uint32_t line_nr, const char* filename, void* /* user_data */) {
  switch (log_level) {
    case 0:  // panic
      SPDLOG_CRITICAL("[{}:{}] {}: {}", filename, line_nr, tag, message);
      break;
    case 1:  // error
      SPDLOG_ERROR("[{}:{}] {}: {}", filename, line_nr, tag, message);
      break;
    case 2:  // warn
      SPDLOG_WARN("[{}:{}] {}: {}", filename, line_nr, tag, message);
      break;
    case 3:  // info
      SPDLOG_INFO("[{}:{}] {}: {}", filename, line_nr, tag, message);
      break;
    default:
      SPDLOG_DEBUG("[{}:{}] {}: {}", filename, line_nr, tag, message);
      break;
  }
}

Renderer::Renderer() : initialized_(false) {}

Renderer::~Renderer() { shutdown(); }

bool Renderer::init() {
  int version = gladLoadGL();
  if (version == 0) {
    SPDLOG_ERROR("Failed to load OpenGL with GLAD");
    return false;
  }

  sg_desc desc{};
  desc.logger.func = custom_sokol_logger;
  sg_setup(&desc);

  if (!sg_isvalid()) {
    SPDLOG_ERROR("Failed to initialize Sokol GFX");
    return false;
  }

  setupTriangle();
  initialized_ = true;
  SPDLOG_INFO("Renderer initialized successfully");
  return true;
}

void Renderer::shutdown() {
  if (initialized_) {
    sg_shutdown();
    initialized_ = false;
  }
}

void Renderer::setupTriangle() {
  // Triangle vertices
  std::array<float, 18> vertices = {{
      0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,    // top (red)
      -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom-left (green)
      0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // bottom-right (blue)
  }};

  // Create vertex buffer
  sg_buffer_desc vbuf_desc{};
  vbuf_desc.data.ptr = vertices.data();
  vbuf_desc.data.size = vertices.size() * sizeof(float);
  vbuf_desc.label = "triangle-vertices";
  vertex_buffer_ = sg_make_buffer(&vbuf_desc);

  // Create shader
  sg_shader_desc shd_desc{};
  shd_desc.vertex_func.source =
      "#version 330\n"
      "layout(location=0) in vec3 position;\n"
      "layout(location=1) in vec3 color;\n"
      "out vec3 v_color;\n"
      "void main() {\n"
      "  gl_Position = vec4(position, 1.0);\n"
      "  v_color = color;\n"
      "}\n";
  shd_desc.fragment_func.source =
      "#version 330\n"
      "in vec3 v_color;\n"
      "out vec4 frag_color;\n"
      "void main() {\n"
      "  frag_color = vec4(v_color, 1.0);\n"
      "}\n";
  shd_desc.label = "triangle-shader";
  sg_shader shd = sg_make_shader(&shd_desc);

  // Create pipeline
  sg_pipeline_desc pip_desc{};
  pip_desc.shader = shd;
  pip_desc.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT3;  // position
  pip_desc.layout.attrs[1].format = SG_VERTEXFORMAT_FLOAT3;  // color
  pip_desc.depth.pixel_format = SG_PIXELFORMAT_DEPTH;
  pip_desc.colors[0].pixel_format = SG_PIXELFORMAT_RGBA8;
  pip_desc.label = "triangle-pipeline";
  pipeline_ = sg_make_pipeline(&pip_desc);
}

void Renderer::beginFrame(int width, int height) {
  sg_pass pass{};
  pass.action.colors[0].load_action = SG_LOADACTION_CLEAR;
  pass.action.colors[0].clear_value = {0.0f, 0.0f, 0.0f, 1.0f};
  pass.swapchain.width = width;
  pass.swapchain.height = height;
  pass.swapchain.sample_count = 1;
  pass.swapchain.color_format = SG_PIXELFORMAT_RGBA8;
  pass.swapchain.depth_format = SG_PIXELFORMAT_DEPTH;

  sg_begin_pass(&pass);
}

void Renderer::render() {
  sg_apply_pipeline(pipeline_);

  sg_bindings bindings{};
  bindings.vertex_buffers[0] = vertex_buffer_;
  sg_apply_bindings(&bindings);

  sg_draw(0, 3, 1);
}

void Renderer::endFrame() {
  sg_end_pass();
  sg_commit();
}
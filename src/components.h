#pragma once
#include <cstdint>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>
#include <vector>

struct Transform {
  glm::vec3 position{0.0f};
  glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
  glm::vec3 scale{1.0f};

  glm::mat4 getMatrix() const {
    glm::mat4 t = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 r = glm::mat4_cast(rotation);
    glm::mat4 s = glm::scale(glm::mat4(1.0f), scale);
    return t * r * s;
  }
};

struct Camera {
  glm::vec3 position{0.0f, 0.0f, 3.0f};
  glm::vec3 target{0.0f, 0.0f, 0.0f};
  glm::vec3 up{0.0f, 1.0f, 0.0f};

  float fov = 45.0f;
  float near_plane = 0.1f;
  float far_plane = 100.0f;

  glm::mat4 getViewMatrix() const { return glm::lookAt(position, target, up); }

  glm::mat4 getProjectionMatrix(float aspect_ratio) const {
    return glm::perspective(glm::radians(fov), aspect_ratio, near_plane, far_plane);
  }
};

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texcoord;
};

struct Mesh {
  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;
  uint32_t vertex_buffer_id = 0;
  uint32_t index_buffer_id = 0;
  bool uploaded = false;

  void clear() {
    vertices.clear();
    indices.clear();
    vertex_buffer_id = 0;
    index_buffer_id = 0;
    uploaded = false;
  }

  size_t getVertexCount() const { return vertices.size(); }
  size_t getIndexCount() const { return indices.size(); }
};
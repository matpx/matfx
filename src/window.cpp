#include "window.h"

Window::Window(int width, int height, const char *title)
    : window_(nullptr), width_(width), height_(height), title_(title) {}

Window::~Window() { shutdown(); }

bool Window::init() {
  if (!glfwInit()) {
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window_ = glfwCreateWindow(width_, height_, title_, nullptr, nullptr);
  if (!window_) {
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(window_);
  glfwSetKeyCallback(window_, keyCallback);

  return true;
}

void Window::shutdown() {
  if (window_) {
    glfwDestroyWindow(window_);
    window_ = nullptr;
  }
  glfwTerminate();
}

bool Window::shouldClose() const { return glfwWindowShouldClose(window_); }

void Window::swapBuffers() { glfwSwapBuffers(window_); }

void Window::pollEvents() { glfwPollEvents(); }

void Window::getFramebufferSize(int &width, int &height) const { glfwGetFramebufferSize(window_, &width, &height); }

void Window::keyCallback(GLFWwindow *window, int key, int /* scancode */, int action, int /* mods */) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}
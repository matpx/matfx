#include "window.h"

#include <spdlog/spdlog.h>

Window::Window(int width, int height, const char *title)
    : window_(nullptr),
      width_(width),
      height_(height),
      title_(title),
      mouse_x_(0.0),
      mouse_y_(0.0),
      mouse_dx_(0.0),
      mouse_dy_(0.0),
      last_mouse_x_(0.0),
      last_mouse_y_(0.0),
      first_mouse_(true) {}

Window::~Window() { shutdown(); }

bool Window::init() {
  if (!glfwInit()) {
    SPDLOG_ERROR("Failed to initialize GLFW");
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window_ = glfwCreateWindow(width_, height_, title_, nullptr, nullptr);
  if (!window_) {
    SPDLOG_ERROR("Failed to create GLFW window");
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(window_);
  glfwSetWindowUserPointer(window_, this);
  glfwSetKeyCallback(window_, keyCallback);
  glfwSetMouseButtonCallback(window_, mouseButtonCallback);
  glfwSetCursorPosCallback(window_, cursorPosCallback);

  SPDLOG_INFO("Window initialized successfully");
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

void Window::pollEvents() {
  updateInputStates();
  glfwPollEvents();
}

void Window::getFramebufferSize(int &width, int &height) const { glfwGetFramebufferSize(window_, &width, &height); }

void Window::keyCallback(GLFWwindow *window, int key, int /* scancode */, int action, int /* mods */) {
  Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }

  if (action == GLFW_PRESS) {
    win->keys_pressed_.insert(key);
    win->keys_just_pressed_.insert(key);
  } else if (action == GLFW_RELEASE) {
    win->keys_pressed_.erase(key);
    win->keys_just_released_.insert(key);
  }
}

void Window::mouseButtonCallback(GLFWwindow *window, int button, int action, int /* mods */) {
  Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));

  if (action == GLFW_PRESS) {
    win->mouse_buttons_pressed_.insert(button);
    win->mouse_buttons_just_pressed_.insert(button);
  } else if (action == GLFW_RELEASE) {
    win->mouse_buttons_pressed_.erase(button);
    win->mouse_buttons_just_released_.insert(button);
  }
}

void Window::cursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
  Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));

  if (win->first_mouse_) {
    win->last_mouse_x_ = xpos;
    win->last_mouse_y_ = ypos;
    win->first_mouse_ = false;
  }

  win->mouse_dx_ = xpos - win->last_mouse_x_;
  win->mouse_dy_ = ypos - win->last_mouse_y_;
  win->last_mouse_x_ = xpos;
  win->last_mouse_y_ = ypos;

  win->mouse_x_ = xpos;
  win->mouse_y_ = ypos;
}

void Window::updateInputStates() {
  keys_just_pressed_.clear();
  keys_just_released_.clear();
  mouse_buttons_just_pressed_.clear();
  mouse_buttons_just_released_.clear();
  mouse_dx_ = 0.0;
  mouse_dy_ = 0.0;
}

bool Window::isKeyPressed(int key) const { return keys_pressed_.find(key) != keys_pressed_.end(); }

bool Window::isKeyJustPressed(int key) const { return keys_just_pressed_.find(key) != keys_just_pressed_.end(); }

bool Window::isKeyJustReleased(int key) const { return keys_just_released_.find(key) != keys_just_released_.end(); }

bool Window::isMouseButtonPressed(int button) const {
  return mouse_buttons_pressed_.find(button) != mouse_buttons_pressed_.end();
}

bool Window::isMouseButtonJustPressed(int button) const {
  return mouse_buttons_just_pressed_.find(button) != mouse_buttons_just_pressed_.end();
}

bool Window::isMouseButtonJustReleased(int button) const {
  return mouse_buttons_just_released_.find(button) != mouse_buttons_just_released_.end();
}

void Window::getMousePosition(double &x, double &y) const {
  x = mouse_x_;
  y = mouse_y_;
}

void Window::getMouseDelta(double &dx, double &dy) const {
  dx = mouse_dx_;
  dy = mouse_dy_;
}
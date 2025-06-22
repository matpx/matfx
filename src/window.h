#pragma once
#include <GLFW/glfw3.h>

#include <unordered_set>

class Window {
 public:
  Window(int width, int height, const char *title);
  ~Window();

  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;
  Window(Window &&) = delete;
  Window &operator=(Window &&) = delete;

  bool init();
  void shutdown();

  bool shouldClose() const;
  void swapBuffers();
  void pollEvents();
  void getFramebufferSize(int &width, int &height) const;

  bool isKeyPressed(int key) const;
  bool isKeyJustPressed(int key) const;
  bool isKeyJustReleased(int key) const;

  bool isMouseButtonPressed(int button) const;
  bool isMouseButtonJustPressed(int button) const;
  bool isMouseButtonJustReleased(int button) const;

  void getMousePosition(double &x, double &y) const;
  void getMouseDelta(double &dx, double &dy) const;

 private:
  static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
  static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
  static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);

  void updateInputStates();
  GLFWwindow *getGLFWWindow() const { return window_; }

  GLFWwindow *window_;
  int width_;
  int height_;
  const char *title_;

  std::unordered_set<int> keys_pressed_;
  std::unordered_set<int> keys_just_pressed_;
  std::unordered_set<int> keys_just_released_;

  std::unordered_set<int> mouse_buttons_pressed_;
  std::unordered_set<int> mouse_buttons_just_pressed_;
  std::unordered_set<int> mouse_buttons_just_released_;

  double mouse_x_, mouse_y_;
  double mouse_dx_, mouse_dy_;
  double last_mouse_x_, last_mouse_y_;
  bool first_mouse_;
};
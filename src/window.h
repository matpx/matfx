#pragma once
#include <GLFW/glfw3.h>

class Window {
public:
    Window(int width, int height, const char* title);
    ~Window();
    
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;
    
    bool init();
    void shutdown();
    
    bool shouldClose() const;
    void swapBuffers();
    void pollEvents();
    void getFramebufferSize(int& width, int& height) const;
    
private:
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    
    GLFWwindow* getGLFWWindow() const { return window_; }
    
    GLFWwindow* window_;
    int width_;
    int height_;
    const char* title_;
};
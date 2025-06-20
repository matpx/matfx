#include "window.h"
#include "renderer.h"

#define SOKOL_IMPL
#include <sokol_log.h>
#include <sokol_gfx.h>

int main() {
    Window window(800, 600, "MatFX");
    if (!window.init()) {
        return -1;
    }

    Renderer renderer;
    if (!renderer.init()) {
        return -1;
    }

    while (!window.shouldClose()) {
        int width, height;
        window.getFramebufferSize(width, height);
        
        renderer.beginFrame(width, height);
        renderer.render();
        renderer.endFrame();
        
        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}
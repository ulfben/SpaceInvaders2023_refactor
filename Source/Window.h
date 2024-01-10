#include "raylib.h"
struct Window{
    Window(std::string_view title, unsigned width, unsigned height, unsigned fps = 60) noexcept {
        InitWindow(width, height, title.data());
        SetTargetFPS(fps);
    }
    ~Window() noexcept {
        CloseWindow();
    }

    bool shouldClose() const noexcept {
        return WindowShouldClose();
    }

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;    
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;
};
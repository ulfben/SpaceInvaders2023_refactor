#include <raylib.h>
#include <stdexcept>
#include <string_view>
#include <string>
struct Window{
    Window(std::string_view title, unsigned width, unsigned height, unsigned fps = 60){
        const std::string tmp(title);
        InitWindow(width, height, tmp.c_str());
        if(!IsWindowReady()){
            throw std::runtime_error("Unable to open Raylib window. Check config?");
        }
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
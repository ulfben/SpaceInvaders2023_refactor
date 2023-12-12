#include <print>
#include <stdexcept>
#include "raylib.h"
#include "game.h"

constexpr int FPS = 60;
constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
constexpr std::string_view TITLE = "Space Invaders";

struct Window{
    Window(std::string_view title, unsigned width, unsigned height, unsigned fps = 60) noexcept {
        InitWindow(width, height, title.data());
        SetTargetFPS(fps);
    }
    ~Window() noexcept {
        CloseWindow();
    }
};
struct Draw {
    Draw() noexcept {
        BeginDrawing();
        ClearBackground(BLACK);
    }
    ~Draw() noexcept {
        EndDrawing();
    }
};

int main(void) {
    try {
        Window w{TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, FPS};        
        Game game{};
        while (!WindowShouldClose()) {
            game.Update();
            Draw d{}; //TODO: make game responsible for this.
            game.Render();        
        }        
    }
    catch (const std::runtime_error& e) {
        std::println("{}", e.what());
    }
    catch (...) {
        std::println("Unkown error!");
    }
    return 0;
}
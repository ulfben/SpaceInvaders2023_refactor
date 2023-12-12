#include <print>
#include <stdexcept>
#include "raylib.h"
#include "game.h"

constexpr int fps = 60;
constexpr int screenWidth = 1920;
constexpr int screenHeight = 1080;
constexpr std::string_view title = "Space Invaders"; 

int main(void) { 
    try {
        InitWindow(screenWidth, screenHeight, title.data());
        SetTargetFPS(fps);                
        Game game = { State::STARTSCREEN };        
        game.Launch();
        while (!WindowShouldClose()){            
            game.Update();            
            BeginDrawing();
            ClearBackground(BLACK);
            game.Render();
            EndDrawing();            
        }        
        CloseWindow();        
    }
    catch (const std::runtime_error& e) {
        std::println("{}", e.what());
    }
    catch (...) {
        std::println("Unkown error!");
    }
    return 0;
}
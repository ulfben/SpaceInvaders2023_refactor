#include "GameScreen.h"
#include "raylib.h"
#include "StartScreen.h"
#include "State.h"
#include <memory>

std::unique_ptr<State> StartScreen::update() noexcept(false){
    if(IsKeyReleased(KEY_SPACE)){
        return std::make_unique<GameScreen>();
    }
    return nullptr;
}

void StartScreen::render() const noexcept{
    DrawText("SPACE INVADERS", 200, 100, 160, YELLOW);
    DrawText("PRESS SPACE TO BEGIN", 200, 350, 40, YELLOW);
}

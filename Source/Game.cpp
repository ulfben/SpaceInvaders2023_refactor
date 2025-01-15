#include "Game.h"
#include "DrawGuard.h"

void Game::run() {
    while(!w.shouldClose()){
        Update();
        Render();
    }
}

void Game::Update() noexcept(false) {
    auto next = state->update();
    if(next){
        state.reset(next.release());
    }
}

void Game::Render() const noexcept{
    DrawGuard d{};
    state->render();
}

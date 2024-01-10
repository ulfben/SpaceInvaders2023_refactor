#include "game.h"
#include <cassert>
#include <cmath>
#include <vector>
#include <span>
#include <algorithm>
struct DrawGuard{
    DrawGuard() noexcept{
        BeginDrawing();
        ClearBackground(BLACK);
    }
    ~DrawGuard() noexcept{
        EndDrawing();
    }
    DrawGuard(const DrawGuard&) = delete;
    DrawGuard& operator=(const DrawGuard&) = delete;
    DrawGuard(DrawGuard&&) = delete;
    DrawGuard& operator=(DrawGuard&&) = delete;
};


void Game::run() noexcept{
    while(!w.shouldClose()){
        Update();
        Render();
    }
}

void Game::Update() noexcept{
    auto next = state->update();
    if(next){
        state.reset(next.release());
    }
}

void Game::Render() const noexcept{
    DrawGuard d{};
    state->render();
}

//bool Game::CheckNewHighScore() noexcept{ 
//    return false;
//    //return (score > Leaderboard.back().score);
//}
//
//void Game::InsertNewHighScore(const std::string& name) noexcept{
//    /*Leaderboard.emplace_back(name, score);
//    std::ranges::sort(Leaderboard, [](const auto& a, const auto& b){
//        return a.score < b.score;
//        });
//    Leaderboard.pop_back();*/
//}
//
//void Game::LoadLeaderboard(){
//  //TODO: implement leaderboard loading
//}
//
//void Game::SaveLeaderboard(){
//  //TODO: implement leaderboard saving
//}
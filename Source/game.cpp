#include "game.h"
#include <cassert>
#include <cmath>
#include <vector>
#include <span>
#include <algorithm>
struct Draw{
    Draw() noexcept{
        BeginDrawing();
        ClearBackground(BLACK);
    }
    ~Draw() noexcept{
        EndDrawing();
    }
};


void Game::run(){
    while(!WindowShouldClose()){
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
    Draw d{};
    state->render();
}



bool Game::CheckNewHighScore(){ //TODO: remove 
    return false;
    //return (score > Leaderboard[4].score);
}

void Game::InsertNewHighScore(const std::string& name){
    /*Leaderboard.emplace_back(name, score);
    std::ranges::sort(Leaderboard, [](const auto& a, const auto& b){
        return a.score < b.score;
        });
    Leaderboard.pop_back();*/
}

void Game::LoadLeaderboard(){
  //TODO: implement leaderboard loading
}

void Game::SaveLeaderboard(){
  //TODO: implement leaderboard saving
}
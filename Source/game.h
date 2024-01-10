#pragma once
#include <string_view>
#include <memory>
#include "State.h"
#include "Window.h"
constexpr unsigned FPS = 60;
constexpr unsigned WINDOW_WIDTH = 1280;
constexpr unsigned WINDOW_HEIGHT = 720;
constexpr std::string_view TITLE = "Space Invaders"sv;

struct Game{  
    Window w{TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, FPS};        
    std::unique_ptr<State> state = std::make_unique<StartScreen>();    
    void run();
    
private:
    void Update() noexcept;
    void Render() const noexcept;
    bool CheckNewHighScore();
    void InsertNewHighScore(const std::string& name);
    void LoadLeaderboard();
    void SaveLeaderboard();
};
#pragma once
#include "State.h"

struct PlayerData{
    std::string name;
    int score;
};

struct Game{  
   // State gameState = State::STARTSCREEN;
    std::unique_ptr<State> state = std::make_unique<StartScreen>();
    
    std::vector<PlayerData> Leaderboard = {{"Player 1", 500}, {"Player 2", 400}, {"Player 3", 300}, {"Player 4", 200}, {"Player 5", 100}};
    
    char name[9 + 1] = "\0";
    int letterCount = 0;
    Rectangle textBox = {600, 500, 225, 50};
    bool mouseOnText = false;
    int framesCounter = 0;    
    bool newHighScore = false;
           
    void Update();
    void Render();
    
    bool CheckNewHighScore();
    void InsertNewHighScore(const std::string& name);
    void LoadLeaderboard();
    void SaveLeaderboard();

};
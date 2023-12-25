#pragma once
#include "State.h"



struct Game{  
    std::unique_ptr<State> state = std::make_unique<StartScreen>();
    
    void run();
    
private:
    void Update();
    void Render();
    bool CheckNewHighScore();
    void InsertNewHighScore(const std::string& name);
    void LoadLeaderboard();
    void SaveLeaderboard();

};
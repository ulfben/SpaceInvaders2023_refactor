#pragma once
#include "raylib.h"
#include <vector>
#include "Resources.h"
#include <string>
#include "Player.h"
#include "Projectile.h"
#include "Wall.h"
#include "Alien.h"
#include "Background.h"
enum struct State{
    STARTSCREEN,
    GAMEPLAY,
    ENDSCREEN
};

struct PlayerData{
    std::string name;
    int score;
};

struct Game{  
    static constexpr unsigned STAR_COUNT = 600;
    static constexpr unsigned WALL_COUNT = 5;
    static constexpr unsigned WALL_DIST_FROM_BOTTOM = 250;

    State gameState = State::STARTSCREEN;
    Resources resources{};
    Background background{STAR_COUNT};
    Player player{};    
    std::vector<Projectile> Projectiles;
    std::vector<Wall> Walls;
    std::vector<Alien> Aliens;
    std::vector<PlayerData> Leaderboard = {{"Player 1", 500}, {"Player 2", 400}, {"Player 3", 300}, {"Player 4", 200}, {"Player 5", 100}};
    int score = 0;


    Vector2 playerPos;
    Vector2 alienPos;
    Vector2 cornerPos;
    
    char name[9 + 1] = "\0";
    int letterCount = 0;
    Rectangle textBox = {600, 500, 225, 50};
    bool mouseOnText = false;
    int framesCounter = 0;
    
    
    float shootTimer = 0;
    Rectangle rec = {0, 0 ,0 ,0};
    int formationWidth = 8;
    int formationHeight = 5;
    int alienSpacing = 80;
    int formationX = 100;
    int formationY = 50;
    bool newHighScore = false;
           
    void End();
    void Continue();
    void Update();
    void Render();
    
    bool CheckNewHighScore();
    void InsertNewHighScore(std::string name);
    void LoadLeaderboard();
    void SaveLeaderboard();
private: 
    void SpawnWalls();
    void SpawnAliens(); 
};
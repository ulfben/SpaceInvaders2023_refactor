#pragma once
#include "raylib.h"
#include <vector>
#include "Resources.h"
#include <string>
#include "Player.h"

enum struct State{
    STARTSCREEN,
    GAMEPLAY,
    ENDSCREEN
};



struct PlayerData{
    std::string name;
    int score;
};

struct Projectile{
    Vector2 position = {0,0};
    int speed = 15;
    bool active = true;
    EntityType type = {};
    Vector2 lineStart = {0, 0};
    Vector2 lineEnd = {0, 0};
    void Update();
    void Render(Texture2D texture);
};

struct Wall{
    Vector2 position;
    Rectangle rec;
    bool active;
    Color color;
    int health = 50;
    int radius = 60;
    void Render(Texture2D texture);
    void Update();
};

struct Alien{
    EntityType type = EntityType::ENEMY;
    Color color = WHITE;
    Vector2 position = {0, 0};
    int x = 0;
    int y = 0;
    float radius = 30;
    bool active = true;
    bool moveRight = true;
    int speed = 2;
    void Update();
    void Render(Texture2D texture);
};


struct Star{
    Vector2 initPosition = {0, 0};
    Vector2 position = {0, 0};
    Color color = GRAY;
    float size = 0;
    void Update(float starOffset);
    void Render();
};

struct Background{
    std::vector<Star> Stars;
    void Initialize(int starAmount);
    void Update(float offset);
    void Render();
};

struct Game{    
    State gameState = State::STARTSCREEN;
    Resources resources{};
    Player player;
    Background background;
    std::vector<Projectile> Projectiles;
    std::vector<Wall> Walls;
    std::vector<Alien> Aliens;
    std::vector<PlayerData> Leaderboard = {{"Player 1", 500}, {"Player 2", 400}, {"Player 3", 300}, {"Player 4", 200}, {"Player 5", 100}};
    
    Vector2 playerPos;
    Vector2 alienPos;
    Vector2 cornerPos;
    float offset;    
    char name[9 + 1] = "\0";
    int letterCount = 0;
    Rectangle textBox = {600, 500, 225, 50};
    bool mouseOnText = false;
    int framesCounter = 0;
    int score;
    int wallCount = 5;
    float shootTimer = 0;
    Rectangle rec = {0, 0 ,0 ,0};
    int formationWidth = 8;
    int formationHeight = 5;
    int alienSpacing = 80;
    int formationX = 100;
    int formationY = 50;
    bool newHighScore = false;


    void Start();
    void End();
    void Continue();
    void Update();
    void Render();
    void SpawnAliens();
    bool CheckCollision(Vector2 circlePos, float circleRadius, Vector2 lineTop, Vector2 lineBottom);
    bool CheckNewHighScore();
    void InsertNewHighScore(std::string name);
    void LoadLeaderboard();
    void SaveLeaderboard();
};
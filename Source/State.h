#pragma once
#include "raylib.h"
#include <vector>
#include "Resources.h"
#include "Player.h"
#include "Projectile.h"
#include "Wall.h"
#include "Alien.h"
#include "Background.h"

struct State{
    virtual State* update() noexcept = 0;
    virtual void render() const noexcept = 0;
    virtual ~State() noexcept = default;
};

struct Gameplay : public State{
    Resources resources{};
    Background background{STAR_COUNT};
    Player player{};    
    int score = 0;
    float shootTimer = 0;
    std::vector<Projectile> playerProjectiles;
    std::vector<Projectile> alienProjectiles;
    std::vector<Wall> Walls;
    std::vector<Alien> Aliens;
    Gameplay();
    State* update() noexcept override;
    void render() const noexcept override;
};

struct StartScreen : public State{
    State* update() noexcept override{
        if(IsKeyReleased(KEY_SPACE)){
            return new Gameplay();
        }
        return nullptr;
    }
    void render() const noexcept override{
        DrawText("SPACE INVADERS", 200, 100, 160, YELLOW);
        DrawText("PRESS SPACE TO BEGIN", 200, 350, 40, YELLOW);
    }
};
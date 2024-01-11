#include "raylib.h"
#include "Resources.h"
#include "State.h"
#include <cassert>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <span>
#include <vector>
#include "RNG.h"

template<typename Container>
static const auto& random(const Container& container) noexcept{    
    static RNG random(createSeeds()); //Note: I absolutely should have used raylibs' GetRandomValue, but I needed to test my own RNG so... 
    const auto index = random.inRange(container.size());
    [[gsl::suppress(bounds.4, justification: "index is guarantueed to be valid.")]]
    return container[index];
}

static constexpr bool is_dead(const auto& p) noexcept{
    return !p.isAlive();
};

template<typename T>
static constexpr void render_all(std::span<const T> entities, const OwnTexture& tex) noexcept{
    for(const T& e : entities){
        e.Render(tex.get());
    }
};

template<typename T>
static constexpr void update_all(std::span<T> entities) noexcept{
    for(T& e : entities){
        e.Update();
    }
};

void Spawn(std::vector<Wall>& Walls){
    constexpr float totalWallWidth = WALL_COUNT * Wall::WIDTH;
    const float spaceAvailable = GetScreenWidthF() - totalWallWidth;
    const float spacing = spaceAvailable / (WALL_COUNT + 1);
    const float y = GetScreenHeightF() - WALL_DIST_FROM_BOTTOM;
    Walls.reserve(WALL_COUNT);
    for(unsigned i = 0; i < WALL_COUNT; i++){
        const float x = spacing * toFloat(i + 1) + toFloat(Wall::WIDTH * i);
        Walls.emplace_back(x, y);
    }
}

void Spawn(std::vector<Alien>& Aliens){
    Aliens.reserve(ALIEN_COUNT);
    constexpr auto formationWidth = ALIEN_COLUMNS * ALIEN_SPACING;
    const auto left = (GetScreenWidth() / 2) - (formationWidth / 2);
    for(unsigned row = 0; row < ALIEN_ROWS; row++){
        for(unsigned col = 0; col < ALIEN_COLUMNS; col++){
            const auto x = left + (col * Alien::WIDTH);
            const auto y = ALIEN_FORMATION_TOP + (row * Alien::HEIGHT);
            Aliens.emplace_back(toFloat(x), toFloat(y));
        }
    }
}

Gameplay::Gameplay(){
    Spawn(walls);
    Spawn(aliens);
}

bool Gameplay::isGameOver() const noexcept{
    const auto reachedPlayer = [pt = player.top()](const Alien& a) noexcept{ return a.bottom() > pt; };
    return IsKeyReleased(KEY_Q) || (player.lives < 1) || aliens.empty() ||
        std::ranges::any_of(aliens, reachedPlayer);
}

std::unique_ptr<State> Gameplay::update() noexcept{
    player.Update();
    background.Update(player.x() / GetScreenWidthF());
    update_all<Alien>(aliens);
    updateAlienProjectiles();
    updatePlayerProjectiles();
    maybePlayerShoots();
    maybeAliensShoots();
    eraseDeadEntities();
    return isGameOver() ? std::make_unique<EndScreen>(score) : nullptr;
}

void Gameplay::render() const noexcept{
    background.Render();
    player.Render();
    render_all<Projectile>(alienProjectiles, resources.laserTexture);
    render_all<Projectile>(playerProjectiles, resources.laserTexture);
    render_all<Wall>(walls, resources.barrierTexture);
    render_all<Alien>(aliens, resources.alienTexture);
    DrawText(TextFormat("Score: %i", score), 50, 20, 40, YELLOW);
    DrawText(TextFormat("Lives: %i", player.lives), 50, 70, 40, YELLOW);
}

void Gameplay::updateAlienProjectiles() noexcept{
    for(auto& ap : alienProjectiles){
        ap.Update();        
        if(collidesWithWalls(ap.hitBox()) || collidesWithPlayer(ap.hitBox())){
            ap.active = false;
        }
    }
}

void Gameplay::updatePlayerProjectiles() noexcept{
    for(auto& pp : playerProjectiles){
        pp.Update();        
        if(collidesWithWalls(pp.hitBox()) || collidesWithAliens(pp.hitBox())){
            pp.active = false;
        }
    }
}

bool Gameplay::collidesWithWalls(const Rectangle& r) noexcept{
    for(auto& w : walls){
        if(CheckCollisionRecs(w.hitBox(), r)){
            w.health--;
            return true;
        }
    }
    return false;
}

bool Gameplay::collidesWithAliens(const Rectangle& r) noexcept{
    for(auto& a : aliens){
        if(CheckCollisionRecs(a.hitBox(), r)){            
            a.active = false;
            score += POINTS_PER_ALIEN;
            return true;
        }
    }
    return false;
}

bool Gameplay::collidesWithPlayer(const Rectangle& r) noexcept{
    if(CheckCollisionRecs(player.hitbox(), r)){
        player.lives--;
        return true;
    }
    return false;
}

void Gameplay::maybePlayerShoots() noexcept{
    if(!IsKeyPressed(KEY_SPACE)){
        return;
    }
    try{
        playerProjectiles.emplace_back(player.gunPosition(), -Projectile::SPEED);
    } catch(...){/*swallowing the exception. The game can keep running without this projectile*/ }
}

void Gameplay::maybeAliensShoots() noexcept{
    if(alienShotCooldown-- || aliens.empty()){
        return;
    }
    alienShotCooldown = ALIEN_SHOT_COOLDOWN;
    try{
        alienProjectiles.emplace_back(random(aliens).gunPosition());
    } catch(...){/*swallowing the exception. The game can keep running without this projectile*/ }
}

void Gameplay::eraseDeadEntities() noexcept{
    std::erase_if(playerProjectiles, is_dead<Projectile>);
    std::erase_if(alienProjectiles, is_dead<Projectile>);
    std::erase_if(aliens, is_dead<Alien>);
    std::erase_if(walls, is_dead<Wall>);
}
//void exit() noexcept override{
//     //TODO: save score, update scoreboard
//    //newHighScore = CheckNewHighScore();
//    //gameState = State::ENDSCREEN;
//}
#include "raylib.h"
#include "Resources.h"
#include "State.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <span>
#include <vector>
static constexpr bool is_dead(const auto& p) noexcept{
    return !p.active;
};

template<typename T>
static constexpr void render_all(std::span<const T> entities, const OwnTexture& tex) noexcept{
    for(const T& e : entities){
        e.Render(tex.get());
    }
};

static constexpr float distanceSq(const Vector2& A, const Vector2& B) noexcept{
    return (B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y);
}

static float distance(const Vector2& A, const Vector2& B) noexcept{
    return std::sqrt(distanceSq(A, B));
}

static constexpr bool pointInCircle(const Vector2& circlePos, float radius, const Vector2& point) noexcept{
    return distanceSq(circlePos, point) < (radius * radius);
}

static constexpr Vector2 closestPointOnLine(const Vector2& A, const Vector2& B, const Vector2& P) noexcept{
    const Vector2 AP = {P.x - A.x, P.y - A.y};
    const Vector2 AB = {B.x - A.x, B.y - A.y};
    const float ab2 = AB.x * AB.x + AB.y * AB.y;
    const float ap_ab = AP.x * AB.x + AP.y * AB.y;
    const float t = std::clamp(ap_ab / ab2, 0.0f, 1.0f);
    return {A.x + AB.x * t, A.y + AB.y * t};
}

static constexpr bool CheckCollision(const Vector2& circlePos, float circleRadius, const Vector2& lineStart, const Vector2& lineEnd) noexcept{
    if(pointInCircle(circlePos, circleRadius, lineStart) || pointInCircle(circlePos, circleRadius, lineEnd)){
        return true;
    }
    Vector2 closest = closestPointOnLine(lineStart, lineEnd, circlePos);
    return pointInCircle(circlePos, circleRadius, closest);
}

void Spawn(std::vector<Wall>& Walls){
    const auto spacing = GetScreenWidthF() / (WALL_COUNT + 1);
    const auto y = GetScreenHeightF() - WALL_DIST_FROM_BOTTOM;
    Walls.reserve(WALL_COUNT);
    for(unsigned i = 0; i < WALL_COUNT; i++){
        const auto x = spacing * toFloat(1 + i);
        Walls.emplace_back(x, y);
    }
}
void Spawn(std::vector<Alien>& Aliens){
    for(int row = 0; row < formationHeight; row++){
        for(int col = 0; col < formationWidth; col++){
            const auto x = formationX + 450 + (col * alienSpacing);
            const auto y = formationY + (row * alienSpacing);
            Aliens.emplace_back(toFloat(x), toFloat(y));
        }
    }
}


Gameplay::Gameplay(){
    Spawn(Walls);
    Spawn(Aliens);
}

bool Gameplay::isGameOver() const noexcept{
    return IsKeyReleased(KEY_Q) || (player.lives < 1) || Aliens.empty();
}
State* Gameplay::update() noexcept{
    if(isGameOver()){
        return new EndScreen();
    }
    player.Update();
    background.Update(player.pos.x / GetScreenWidthF());
    for(auto& a : Aliens){
        a.Update();
        if(a.position.y > player.pos.y){
            return new EndScreen();
        }
    }
    for(auto& p : alienProjectiles){
        p.Update();
        for(auto& w : Walls){
            if(CheckCollision(w.position, Wall::RADIUS, p.lineStart, p.lineEnd)){
                p.active = false;
                w.health -= 1;
                break;
            }
        }
        if(!p.active){
            continue;
        }
        if(CheckCollision(player.pos, player.radius, p.lineStart, p.lineEnd)){
            p.active = false;
            player.lives -= 1;
        }
    }
    for(auto& p : playerProjectiles){
        p.Update();
        for(auto& w : Walls){
            if(CheckCollision(w.position, Wall::RADIUS, p.lineStart, p.lineEnd)){
                p.active = false;
                w.health -= 1;
                break;
            }
        }
        if(!p.active){
            continue;
        }
        for(auto& a : Aliens){
            if(CheckCollision(a.position, Alien::RADIUS, p.lineStart, p.lineEnd)){
                p.active = false;
                a.active = false;
                score += 100;
                break;
            }
        }

    }
    for(auto& w : Walls){
        w.Update();
    }

    if(IsKeyPressed(KEY_SPACE)){
        auto pos = player.pos;
        pos.y -= 30.0f;
        playerProjectiles.emplace_back(pos, -Projectile::SPEED);
    }

    shootTimer += 1;
    if(shootTimer > 59 && !Aliens.empty()){
        const int i = rand() % Aliens.size();
        auto pos = Aliens[i].position;
        pos.y += 40;
        alienProjectiles.emplace_back(pos);
        shootTimer = 0;
    }
    std::erase_if(playerProjectiles, is_dead<Projectile>);
    std::erase_if(alienProjectiles, is_dead<Projectile>);
    std::erase_if(Aliens, is_dead<Alien>);
    std::erase_if(Walls, is_dead<Wall>);
    return nullptr;
}
void Gameplay::render() const noexcept{
    const auto i = player.activeTexture;
    background.Render();
    player.Render(resources.shipTextures[i].get());
    render_all<Projectile>(alienProjectiles, resources.laserTexture);
    render_all<Projectile>(playerProjectiles, resources.laserTexture);
    render_all<Wall>(Walls, resources.barrierTexture);
    render_all<Alien>(Aliens, resources.alienTexture);
    DrawText(TextFormat("Score: %i", score), 50, 20, 40, YELLOW);
    DrawText(TextFormat("Lives: %i", player.lives), 50, 70, 40, YELLOW);
}

//void exit() noexcept override{
//     //TODO: save score, update scoreboard
//    //newHighScore = CheckNewHighScore();
//    //gameState = State::ENDSCREEN;
//}

#pragma once
#include "Alien.h"
#include "AutoTexture.h"
#include "Background.h"
#include "Player.h"
#include "Projectile.h"
#include "UtilsRaylib.h"
#include "Settings.h"
#include "State.h"
#include "Wall.h"
#include <memory>
#include <vector>

class GameScreen : public State{
public:
    GameScreen() noexcept(false);
    std::unique_ptr<State> update() noexcept(false) override;
    void render() const noexcept override;

private:
    //Note: the Player class owns its own textures, so they are not included here.
    AutoTexture alien_gfx = AutoTexture{"./Assets/Alien.png"sv};
    AutoTexture wall_gfx = AutoTexture{"./Assets/Barrier.png"sv};
    AutoTexture beam_gfx = AutoTexture{"./Assets/Laser.png"sv};
    Background background{STAR_COUNT};
    Player player{};    
    std::vector<Projectile> playerProjectiles;
    std::vector<Projectile> alienProjectiles;
    std::vector<Wall> walls;
    std::vector<Alien> aliens;
    int score{0};
    int alienShotCooldown{ALIEN_SHOT_COOLDOWN};

    bool isGameOver() const noexcept;
    void updateAlienProjectiles() noexcept;
    void updatePlayerProjectiles() noexcept;
    bool collidesWithPlayer(const Rectangle& r) noexcept;
    bool collidesWithWalls(const Rectangle& r) noexcept;
    bool collidesWithAliens(const Rectangle& r) noexcept;
    void maybeAliensShoots() noexcept;
    void maybePlayerShoots() noexcept;
    void eraseDeadEntities() noexcept;
};
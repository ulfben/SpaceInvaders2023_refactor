#pragma once
#include "raylib.h"
#include <vector>
#include <string>
#include "Resources.h"
#include "Player.h"
#include "Projectile.h"
#include "Wall.h"
#include "Alien.h"
#include "Background.h"
using namespace std::string_literals;

struct State{
    virtual std::unique_ptr<State> update() noexcept = 0;
    virtual void render() const noexcept = 0;
    virtual ~State() noexcept = default;
};

struct Gameplay : public State{
    Resources resources{};
    Background background{STAR_COUNT};
    Player player{};
    int score = 0;
    int alienShotCooldown = ALIEN_SHOT_COOLDOWN;
    std::vector<Projectile> playerProjectiles;
    std::vector<Projectile> alienProjectiles;
    std::vector<Wall> walls;
    std::vector<Alien> aliens;
    Gameplay();
    std::unique_ptr<State> update() noexcept override;
    void render() const noexcept override;
private:
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

struct StartScreen : public State{
    std::unique_ptr<State> update() noexcept override{
        if(IsKeyReleased(KEY_SPACE)){
            return std::make_unique<Gameplay>();
        }
        return nullptr;
    }
    void render() const noexcept override{
        DrawText("SPACE INVADERS", 200, 100, 160, YELLOW);
        DrawText("PRESS SPACE TO BEGIN", 200, 350, 40, YELLOW);
    }
};

struct EndScreen : public State{    
    explicit EndScreen(int score);    
    std::unique_ptr<State> update() noexcept override;
    void render() const noexcept override;

private: 
    static const size_t MAX_INPUT_SIZE = 3;
    void loadScores(std::string_view path);
    void addNewScore();
    void saveScores(std::string_view path) noexcept;   
    bool mouseOnTextfield() const noexcept;
    bool isNewHighscore() const noexcept;
    bool isEntryComplete() const noexcept;
    void doTextEntry() noexcept;
    void sortTable() noexcept;
    struct ScoreEntry{
        std::string name;
        int score = 0;
    };
    ScoreEntry current;
    std::vector<ScoreEntry> highscores = {{"AAA"s, 0}, {"BBB"s, 0}, {"CCC"s, 0}, {"DDD"s, 0}, {"EEE"s, 0}};
    std::string name;    
    Rectangle textBox = {600, 500, 225, 50};    
};
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

struct PlayerData{
    std::string name;
    int score;
};

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
    std::vector<PlayerData> Leaderboard = {{"Player 1", 500}, {"Player 2", 400}, {"Player 3", 300}, {"Player 4", 200}, {"Player 5", 100}};
    std::string name;
    Rectangle textBox = {600, 500, 225, 50};
    bool newHighScore = false;

    bool mouseOnText() const noexcept{
        return CheckCollisionPointRec(GetMousePosition(), textBox);
    }

    std::unique_ptr<State> update() noexcept override{
        if(IsKeyReleased(KEY_ENTER) && !newHighScore){
            //TODO: save leaderboard
            return std::make_unique<StartScreen>();
        }
    /*    if(!newHighScore){
            return nullptr;
        }*/
        if(mouseOnText()){
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
            for(int key = GetCharPressed(); key > 0; key = GetCharPressed()){
                if(key > 31 && key < 126 && name.size() < 3){
                    name.push_back(static_cast<char>(key));
                }
            }
            if(IsKeyPressed(KEY_BACKSPACE) && !name.empty()){
                name.pop_back();
            }
        } else{
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }
        if(!name.empty() && IsKeyReleased(KEY_ENTER)){
            //InsertNewHighScore(nameEntry);
            newHighScore = false;
        }

        return nullptr;
    }

    void render() const noexcept override{
        /*if(!newHighScore){
            DrawText("PRESS ENTER TO CONTINUE", 600, 200, 40, YELLOW);
            DrawText("LEADERBOARD", 50, 100, 40, YELLOW);
            for(int i = 0; i < Leaderboard.size(); i++){
                DrawText(Leaderboard[i].name.c_str(), 50, 140 + (i * 40), 40, YELLOW);
                DrawText(TextFormat("%i", Leaderboard[i].score), 350, 140 + (i * 40), 40, YELLOW);
            }
            return;
        }*/
        DrawText("NEW HIGHSCORE!", 600, 300, 60, YELLOW);
        DrawText("PLACE MOUSE OVER INPUT BOX!", 600, 400, 20, YELLOW);
        DrawRectangleRec(textBox, LIGHTGRAY);
        const auto x = toInt<int>(textBox.x);
        const auto y = toInt<int>(textBox.y);
        const auto color = mouseOnText() ? RED : DARKGRAY;
        DrawRectangleLines(x, y, (int) textBox.width, (int) textBox.height, color);
        DrawText(name.c_str(), x + 5, y + 8, 40, MAROON);       
        if(!name.empty()){
            DrawText("PRESS ENTER TO CONTINUE", 600, 800, 40, YELLOW);
        }
    }
};
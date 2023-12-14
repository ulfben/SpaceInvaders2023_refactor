#include "game.h"
#include <cassert>
#include <cmath>
#include <vector>
#include <span>
struct Draw{
    Draw() noexcept{
        BeginDrawing();
        ClearBackground(BLACK);
    }
    ~Draw() noexcept{
        EndDrawing();
    }
};

bool is_dead(const auto& p){
    return !p.active;
};

template<typename T>
static void render(std::span<const T> entities, const OwnTexture& tex) noexcept{
    for(const T& e : entities){
        e.Render(tex.get());
    }
};

float distanceSq(const Vector2& A, const Vector2& B) noexcept{
    return (B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y);
}

float distance(const Vector2& A, const Vector2& B) noexcept{
    return std::sqrt(distanceSq(A, B));
}

bool pointInCircle(const Vector2& circlePos, float radius, const Vector2& point) noexcept{
    return distanceSq(circlePos, point) < (radius * radius);
}

Vector2 closestPointOnLine(const Vector2& A, const Vector2& B, const Vector2& P) noexcept{
    const Vector2 AP = {P.x - A.x, P.y - A.y};
    const Vector2 AB = {B.x - A.x, B.y - A.y};
    const float ab2 = AB.x * AB.x + AB.y * AB.y;
    const float ap_ab = AP.x * AB.x + AP.y * AB.y;
    float t = ap_ab / ab2;
    if(t < 0.0f) t = 0.0f;
    else if(t > 1.0f) t = 1.0f;
    return {A.x + AB.x * t, A.y + AB.y * t};
}

bool CheckCollision(const Vector2& circlePos, float circleRadius, const Vector2& lineStart, const Vector2& lineEnd) noexcept{
    if(pointInCircle(circlePos, circleRadius, lineStart) || pointInCircle(circlePos, circleRadius, lineEnd)){
        return true;
    }
    Vector2 closest = closestPointOnLine(lineStart, lineEnd, circlePos);
    return pointInCircle(circlePos, circleRadius, closest);
}

void Game::SpawnWalls(){    
    const auto spacing = GetScreenWidthF() / (WALL_COUNT + 1);
    const auto y = GetScreenHeightF() - WALL_DIST_FROM_BOTTOM;
    Walls.reserve(WALL_COUNT);
    for(unsigned i = 0; i < WALL_COUNT; i++){
        const auto x = spacing * toFloat(1+i);
        Walls.emplace_back(x, y);
    }
}

void Game::End(){
    //TODO: save score, update scoreboard
    Projectiles.clear();
    Walls.clear();
    Aliens.clear();
    newHighScore = CheckNewHighScore();
    gameState = State::ENDSCREEN;
}

void Game::Continue(){
    SaveLeaderboard();
    gameState = State::STARTSCREEN;
}

void Game::Update(){
    switch(gameState){
    case State::STARTSCREEN:
        if(IsKeyReleased(KEY_SPACE)){
            gameState = State::GAMEPLAY; //TODO: the game currently can not restart. I will create a state machine to handle re-init of game state.
            SpawnWalls();
            SpawnAliens();  
        }
        break;
    case State::GAMEPLAY:
        if(IsKeyReleased(KEY_Q)){
            End();
        }
        player.Update();
        for(auto& a : Aliens){
            a.Update();
            if(a.position.y > player.pos.y){
                End();
            }
        }
        if(player.lives < 1){
            End();
        }
        if(Aliens.empty()){
            SpawnAliens();
        }        
        background.Update(player.pos.x / GetScreenWidthF());
        for(auto& p : Projectiles){
            p.Update();
        }
        for(auto& w : Walls){
            w.Update();
        }
        for(auto& p : Projectiles){
            if(p.type == EntityType::PLAYER_PROJECTILE){
                for(auto& a : Aliens){
                    if(CheckCollision(a.position, a.radius, p.lineStart, p.lineEnd)){
                        p.active = false;
                        a.active = false;
                        score += 100;
                    }
                }
            }

            if(p.type == EntityType::ENEMY_PROJECTILE){
                if(CheckCollision(player.pos, player.radius, p.lineStart, p.lineEnd)){
                    p.active = false;
                    player.lives -= 1;
                }
            }

            for(auto& w : Walls){
                if(CheckCollision(w.position, Wall::RADIUS, p.lineStart, p.lineEnd)){
                    p.active = false;
                    w.health -= 1;
                }
            }
        }

        if(IsKeyPressed(KEY_SPACE)){
            Projectile newProjectile;
            newProjectile.position = player.pos;
            newProjectile.position.y -= 30.0f;
            newProjectile.type = EntityType::PLAYER_PROJECTILE;
            Projectiles.push_back(newProjectile);
        }

        shootTimer += 1;
        if(shootTimer > 59){
            int randomAlienIndex = 0;
            if(!Aliens.empty()){
                randomAlienIndex = rand() % Aliens.size();
            }

            Projectile newProjectile;
            newProjectile.position = Aliens[randomAlienIndex].position;
            newProjectile.position.y += 40;
            newProjectile.speed = -15;
            newProjectile.type = EntityType::ENEMY_PROJECTILE;
            Projectiles.push_back(newProjectile);
            shootTimer = 0;
        }
        std::erase_if(Projectiles, is_dead<Projectile>);
        std::erase_if(Aliens, is_dead<Alien>);
        std::erase_if(Walls, is_dead<Wall>);
        break;
    case State::ENDSCREEN:
        if(IsKeyReleased(KEY_ENTER) && !newHighScore){
            Continue();
        }
        if(newHighScore){
            if(CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
            else mouseOnText = false;
            if(mouseOnText){
                SetMouseCursor(MOUSE_CURSOR_IBEAM);
                int key = GetCharPressed();
                while(key > 0){
                    if((key >= 32) && (key <= 125) && (letterCount < 9)){
                        name[letterCount] = (char) key;
                        name[letterCount + 1] = '\0';
                        letterCount++;
                    }
                    key = GetCharPressed();
                }

                if(IsKeyPressed(KEY_BACKSPACE)){
                    letterCount--;
                    if(letterCount < 0) letterCount = 0;
                    name[letterCount] = '\0';
                }
            } else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

            if(mouseOnText){
                framesCounter++;
            } else{
                framesCounter = 0;
            }
            if(letterCount > 0 && letterCount < 9 && IsKeyReleased(KEY_ENTER)){
                std::string nameEntry(name);
                InsertNewHighScore(nameEntry);
                newHighScore = false;
            }
        }
        break;
    default:
        assert(false && "Game::update() invalid state reached");
        break;
    }
}

void Game::Render(){
    Draw d{};
    switch(gameState){
    case State::STARTSCREEN:
        DrawText("SPACE INVADERS", 200, 100, 160, YELLOW);
        DrawText("PRESS SPACE TO BEGIN", 200, 350, 40, YELLOW);
        break;
    case State::GAMEPLAY:
        background.Render();
        player.Render(resources.shipTextures[player.activeTexture].get());
        render<Projectile>(Projectiles, resources.laserTexture);
        render<Wall>(Walls, resources.barrierTexture);
        render<Alien>(Aliens, resources.alienTexture);
        DrawText(TextFormat("Score: %i", score), 50, 20, 40, YELLOW);
        DrawText(TextFormat("Lives: %i", player.lives), 50, 70, 40, YELLOW);
        break;
    case State::ENDSCREEN:
        if(newHighScore){
            DrawText("NEW HIGHSCORE!", 600, 300, 60, YELLOW);
            DrawText("PLACE MOUSE OVER INPUT BOX!", 600, 400, 20, YELLOW);
            DrawRectangleRec(textBox, LIGHTGRAY);
            if(mouseOnText){
                DrawRectangleLines((int) textBox.x, (int) textBox.y, (int) textBox.width, (int) textBox.height, RED);
            } else{
                DrawRectangleLines((int) textBox.x, (int) textBox.y, (int) textBox.width, (int) textBox.height, DARKGRAY);
            }
            DrawText(name, (int) textBox.x + 5, (int) textBox.y + 8, 40, MAROON);
            DrawText(TextFormat("INPUT CHARS: %i/%i", letterCount, 8), 600, 600, 20, YELLOW);

            if(mouseOnText){
                if(letterCount < 9){
                    if(((framesCounter / 20) % 2) == 0){ //blinking caret position 
                        DrawText("_", (int) textBox.x + 8 + MeasureText(name, 40), (int) textBox.y + 12, 40, MAROON);
                    }
                } else{
                    DrawText("Press BACKSPACE to delete chars...", 600, 650, 20, YELLOW);
                }
            }
            if(letterCount > 0 && letterCount < 9){
                DrawText("PRESS ENTER TO CONTINUE", 600, 800, 40, YELLOW);
            }

        } else{
            // If no highscore or name is entered, show scoreboard and call it a day
            DrawText("PRESS ENTER TO CONTINUE", 600, 200, 40, YELLOW);
            DrawText("LEADERBOARD", 50, 100, 40, YELLOW);
            for(int i = 0; i < Leaderboard.size(); i++){
                char* tempNameDisplay = Leaderboard[i].name.data();
                DrawText(tempNameDisplay, 50, 140 + (i * 40), 40, YELLOW);
                DrawText(TextFormat("%i", Leaderboard[i].score), 350, 140 + (i * 40), 40, YELLOW);
            }
        }
        break;
    default:
        assert(false && "Game::render() invalid state reached");
        break;
    }
}

void Game::SpawnAliens(){
    for(int row = 0; row < formationHeight; row++){
        for(int col = 0; col < formationWidth; col++){
            Alien newAlien = Alien();
            newAlien.active = true;
            newAlien.position.x = formationX + 450 + (col * alienSpacing);
            newAlien.position.y = formationY + (row * alienSpacing);
            Aliens.push_back(newAlien);
        }
    }
}

bool Game::CheckNewHighScore(){ //TODO: remove 
    return (score > Leaderboard[4].score);
}

void Game::InsertNewHighScore(std::string name){
    PlayerData newData; //TODO: rewrite implementation. push new score, sort list, pop if list is too long.
    newData.name = name;
    newData.score = score;
    for(int i = 0; i < Leaderboard.size(); i++){
        if(newData.score > Leaderboard[i].score){
            Leaderboard.insert(Leaderboard.begin() + i, newData);
            Leaderboard.pop_back();
            i = Leaderboard.size();
        }
    }
}

void Game::LoadLeaderboard(){
  //TODO: implement leaderboard loading
}

void Game::SaveLeaderboard(){
  //TODO: implement leaderboard saving
}
#include "game.h"
#include <cassert>
#include <cmath>
#include <vector>

struct Draw{
    Draw() noexcept{
        BeginDrawing();
        ClearBackground(BLACK);
    }
    ~Draw() noexcept{
        EndDrawing();
    }
};

float distance(Vector2 A, Vector2 B) noexcept{
    return std::sqrtf(std::pow(B.x - A.x, 2.0f) + pow(B.y - A.y, 2.0f));
}

bool pointInCircle(Vector2 circlePos, float radius, Vector2 point) noexcept{
    return distance(circlePos, point) < radius;
}

void Game::Start(){
    float window_width = (float) GetScreenWidth();
    float window_height = (float) GetScreenHeight();
    float wall_distance = window_width / (wallCount + 1);
    for(int i = 0; i < wallCount; i++){
        Wall newWalls;
        newWalls.position.y = window_height - 250;
        newWalls.position.x = wall_distance * (i + 1);
        Walls.push_back(newWalls);
    }
    SpawnAliens();
    Background newBackground;
    newBackground.Initialize(600);
    background = newBackground;
    score = 0;
    gameState = State::GAMEPLAY;
}

void Game::End(){
    //TODO: save score, updare scoreboard
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
            Start();
        }
        break;
    case State::GAMEPLAY:      
        if(IsKeyReleased(KEY_Q)){
            End();
        }        
        player.Update();        
        for(int i = 0; i < Aliens.size(); i++){
            Aliens[i].Update();
            if(Aliens[i].position.y > player.pos.y){
                End();
            }
        }        
        if(player.lives < 1){
            End();
        }        
        if(Aliens.size() < 1){
            SpawnAliens();
        }
                
        offset = distance(player.pos, {0.0f, player.pos.y}) * -1;
        background.Update(offset / 15);

        for(int i = 0; i < Projectiles.size(); i++){
            Projectiles[i].Update();
        }
        for(int i = 0; i < Walls.size(); i++){
            Walls[i].Update();
        }
        for(int i = 0; i < Projectiles.size(); i++){
            if(Projectiles[i].type == EntityType::PLAYER_PROJECTILE){
                for(int a = 0; a < Aliens.size(); a++){
                    if(CheckCollision(Aliens[a].position, Aliens[a].radius, Projectiles[i].lineStart, Projectiles[i].lineEnd)){                      
                        Projectiles[i].active = false;
                        Aliens[a].active = false;
                        score += 100;
                    }
                }
            }

            for(int i = 0; i < Projectiles.size(); i++){
                if(Projectiles[i].type == EntityType::ENEMY_PROJECTILE){
                    if(CheckCollision(player.pos, player.radius, Projectiles[i].lineStart, Projectiles[i].lineEnd)){                        
                        Projectiles[i].active = false;
                        player.lives -= 1;
                    }
                }
            }

            for(int b = 0; b < Walls.size(); b++){
                if(CheckCollision(Walls[b].position, Walls[b].radius, Projectiles[i].lineStart, Projectiles[i].lineEnd)){               
                    Projectiles[i].active = false;
                    Walls[b].health -= 1;
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
        if(shootTimer > 59)
        {
            int randomAlienIndex = 0;

            if(Aliens.size() > 1){
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
               
        for(int i = 0; i < Projectiles.size(); i++){
            if(Projectiles[i].active == false){
                Projectiles.erase(Projectiles.begin() + i);        
                i--;
            }
        }
        for(int i = 0; i < Aliens.size(); i++){
            if(Aliens[i].active == false){
                Aliens.erase(Aliens.begin() + i);
                i--;
            }
        }
        for(int i = 0; i < Walls.size(); i++){
            if(Walls[i].active == false){
                Walls.erase(Walls.begin() + i);
                i--;
            }
        }
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
        DrawText(TextFormat("Score: %i", score), 50, 20, 40, YELLOW);
        DrawText(TextFormat("Lives: %i", player.lives), 50, 70, 40, YELLOW);
        player.Render(resources.shipTextures[player.activeTexture].get());
        
        for(int i = 0; i < Projectiles.size(); i++){
            Projectiles[i].Render(resources.laserTexture.get());
        }        
        for(int i = 0; i < Walls.size(); i++){
            Walls[i].Render(resources.barrierTexture.get());
        }
        for(int i = 0; i < Aliens.size(); i++){
            Aliens[i].Render(resources.alienTexture.get());
        }
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


bool Game::CheckCollision(Vector2 circlePos, float circleRadius, Vector2 lineStart, Vector2 lineEnd){
    if(pointInCircle(circlePos, circleRadius, lineStart) || pointInCircle(circlePos, circleRadius, lineEnd)){
        return true;
    }
    Vector2 A = lineStart;
    Vector2 B = lineEnd;
    Vector2 C = circlePos;
    float length = distance(A, B);
    float dotP = (((C.x - A.x) * (B.x - A.x)) + ((C.y - A.y) * (B.y - A.y))) / pow(length, 2);
    float closestX = A.x + (dotP * (B.x - A.x));
    float closestY = A.y + (dotP * (B.y - A.y));
    float buffer = 0.1f;
    float closeToStart = distance(A, {closestX, closestY});
    float closeToEnd = distance(B, {closestX, closestY});
    float closestLength = closeToStart + closeToEnd;
    if(closestLength == length + buffer || closestLength == length - buffer){
        return (distance(A, {closestX, closestY}) < circleRadius);
    }
    return false;
}
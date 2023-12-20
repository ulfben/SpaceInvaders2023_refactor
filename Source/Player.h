#pragma once
#include "Resources.h"

struct Player{
    EntityType type = EntityType::PLAYER;
    Vector2 pos{GetScreenWidth() / 2.0f, GetScreenHeight()-70.f};
    float speed = 7;
    float radius = 50;
    float timer = 0;
    int lives = 3;
    int direction = 0;
    int activeTexture = 0;
        
    void Update() noexcept {
        direction = 0;
        if(IsKeyDown(KEY_LEFT)){
            direction--;
        }
        if(IsKeyDown(KEY_RIGHT)){
            direction++;
        }

        pos.x += speed * direction;

        if(pos.x < radius){
           pos.x = radius;
        } else if(pos.x > GetScreenWidth() - radius){
            pos.x = GetScreenWidth() - radius;
        }

        timer += GetFrameTime();

        if(timer > 0.4 && activeTexture == 2){
            activeTexture = 0;
            timer = 0;
        } else if(timer > 0.4){
            activeTexture++;
            timer = 0;
        }
    }

    void Render(const Texture2D& texture) const noexcept{        
        DrawTexturePro(texture,
                {0,0,352,352,}, {pos.x, pos.y,100,100,}, 
                {50, 50},0,WHITE
        );
    }

};
#pragma once
#include "Resources.h"

struct Player{
    float x_pos = 0;
    float speed = 7;
    float player_base_height = 70.0f;
    float radius = 50;
    int lives = 3;
    int direction = 0;
    int activeTexture = 0;
    float timer = 0;
    EntityType type = EntityType::PLAYER;
    void Initialize() noexcept {
        float window_width = (float) GetScreenWidth();
        x_pos = window_width / 2;
    }

    void Update() noexcept {
        direction = 0;
        if(IsKeyDown(KEY_LEFT)){
            direction--;
        }
        if(IsKeyDown(KEY_RIGHT)){
            direction++;
        }

        x_pos += speed * direction;

        if(x_pos < 0 + radius){
            x_pos = 0 + radius;
        } else if(x_pos > GetScreenWidth() - radius){
            x_pos = GetScreenWidth() - radius;
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

    void Render(Texture2D texture) const noexcept{
        float window_height = GetScreenHeight();
        DrawTexturePro(texture,
                {0,0,352,352,}, {x_pos, window_height - player_base_height,100,100,}, 
                {50, 50},0,WHITE
        );
    }

};
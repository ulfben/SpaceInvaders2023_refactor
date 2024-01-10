#pragma once
#include "Resources.h"
#include <algorithm> //for std::clamp
struct Player{
    EntityType type = EntityType::PLAYER;
    Vector2 pos{GetScreenWidth() / 2.0f, GetScreenHeight() - 70.f};
    float speed = 7;
    float radius = 50;
    float timer = 0;
    int lives = 3;    
    int activeTexture = 0;

    float x() const noexcept{
        return pos.x;
    }
    float y() const noexcept{
        return pos.y;
    }
    Vector2 gunPosition() const noexcept{
        return {x(), y() - radius};
    }

    void Update() noexcept{
        auto direction = IsKeyDown(KEY_LEFT) ? -1.0f : (IsKeyDown(KEY_RIGHT) ? 1.0f : 0.0f);
        pos.x += speed * direction;
        pos.x = std::clamp(x(), radius, GetScreenWidthF() - radius);        
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
            {50, 50}, 0, WHITE
        );
    }

};
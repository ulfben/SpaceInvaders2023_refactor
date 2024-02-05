#pragma once
#include "Animation.h"
#include "raylib.h"
#include "Settings.h"
#include "UtilsRaylib.h"
#include <algorithm> //for std::clamp

struct Player{
    static constexpr float SPEED = 7;
    static constexpr float WIDTH = 94;
    static constexpr float HEIGHT = 97;
    Animation animation{PlayerAnimationFiles};
    Vector2 position{GetScreenWidthF() / 2.0f, GetScreenHeightF() - HEIGHT};
    int lives = 3;     

    float x() const noexcept{
        return position.x;
    }
    float y() const noexcept{
        return position.y;
    }
    float width() const noexcept{
        return animation.width();
    }
    float height() const noexcept{
        return animation.height();
    }
    float right() const noexcept{
        return x() + width();
    }
    float left() const noexcept{
        return x();
    }
    float top() const noexcept{ 
        return y();
    }
    float centerX() const noexcept{
        return x() + animation.width() / 2.0f;
    }
    Vector2 gunPosition() const noexcept{
        return {centerX(), top() };
    }
    Rectangle hitbox() const noexcept{
        return {x(), y(), animation.width(), animation.height()};
    }

    void Update() noexcept{
        const auto direction = IsKeyDown(KEY_LEFT) ? -1.0f : (IsKeyDown(KEY_RIGHT) ? 1.0f : 0.0f);
        position.x += SPEED * direction;
        position.x = std::clamp(x(), 0.0f, GetScreenWidthF()-width());
        animation.update();
    }

    void Render() const noexcept{        
        DrawTexture(animation.currentFrame(), position);  
    }
};
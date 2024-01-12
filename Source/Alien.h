#pragma once
#include "raylib.h"
#include "UtilsRaylib.h"

struct Alien{            
    static constexpr int WIDTH = 94; //TODO: these should be read from the sprite... 
    static constexpr int HEIGHT = 54;    
    Vector2 position = {0, 0};    
    float velocity = 2.0f;
    bool active = true;

    Alien(float x, float y) noexcept : position{x, y}{}

    float x() const noexcept{
        return position.x;
    }
    float y() const noexcept{
        return position.y;
    }
    float bottom() const noexcept{
        return y() + HEIGHT;
    }
    float right() const noexcept{
        return x() + WIDTH;
    }
    float centerX() const noexcept{
        return x() + WIDTH / 2.0f;
    }
    
    Vector2 gunPosition() const noexcept{
        return {centerX(), bottom()};
    }
    
    Rectangle hitBox() const noexcept{
        return {x(), y(), WIDTH, HEIGHT};
    }

    void Update() noexcept{
        position.x += velocity;
        if(should_turn()){
            velocity *= -1.0f;
            position.y += HEIGHT;
        }
    }

    void Render(const Texture2D& texture) const noexcept{
        DrawTexture(texture, position);
    }

    bool should_turn() const noexcept{
        return (x() < 0 || right() > GetScreenWidthF());
    }

    bool isAlive() const noexcept{
        return active;
    }
};
#include "Resources.h"

struct Alien{    
    static constexpr float SPEED = 2.0f;
    static constexpr float RADIUS = 30.0f;
    static constexpr float GRID = 50.0f;    
    
    Vector2 position = {0, 0};
    bool active = true;
    float velocity = SPEED;

    Alien(float x, float y) noexcept : position{x, y}{}

    float x() const noexcept{
        return position.x;
    }
    float y() const noexcept{
        return position.y;
    }
    Vector2 gunPosition() const noexcept{
        return {x(), y() + RADIUS};
    }
    

    void Update() noexcept{
        position.x += velocity;
        if(should_turn()){
            velocity *= -1.0f;
            position.y += GRID;
        }
    }

    void Render(const Texture2D& texture) const noexcept{
        DrawTexturePro(texture,
            {0,0,352,352,}, {x(), y(), 100, 100},
            {50 , 50}, 0, WHITE
        );
    }

    bool should_turn() const noexcept{
        return (x() < RADIUS || x() > GetScreenWidthF() - RADIUS);
    }

    bool isAlive() const noexcept{
        return active;
    }
};
#include "Resources.h"
struct Projectile{
    static constexpr auto SPEED = 15.0f; 
    Vector2 position{0,0};
    float speed = SPEED;
    bool active = true;    
    Vector2 lineStart = {0, 0};
    Vector2 lineEnd = {0, 0};
    Projectile(Vector2 pos, float velocity = SPEED) noexcept : position(pos), speed(velocity)
    {}
    void Update() noexcept{
        position.y += speed;        
        lineStart.y = position.y - 15;
        lineEnd.y = position.y + 15;
        lineStart.x = position.x;
        lineEnd.x = position.x;
        if(position.y < 0.0f || position.y > GetScreenHeightF()){
            active = false;
        }
    }

    void Render(const Texture2D& texture) const noexcept{      
        DrawTexturePro(texture,
            {0,0,176,176,}, {position.x,position.y,50,50,}, 
            {25 , 25}, 0, WHITE
        );
    }

    bool isAlive() const noexcept{
        return active;
    }
};
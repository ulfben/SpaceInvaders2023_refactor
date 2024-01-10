#include "Resources.h"
struct Projectile{
    static constexpr auto SPEED = 15.0f; 
    static constexpr auto RADIUS = 25.0f; 
    Vector2 position{0,0};
    float speed = SPEED;
    bool active = true;     
    Projectile(Vector2 pos, float velocity = SPEED) noexcept : position(pos), speed(velocity)
    {}
    void Update() noexcept{
        position.y += speed;        
        if(position.y < -RADIUS || position.y > GetScreenHeightF()+RADIUS){
            active = false;
        }
    }

    void Render(const Texture2D& texture) const noexcept{      
        DrawTexture(texture, position);      
    }

    bool isAlive() const noexcept{
        return active;
    }
};
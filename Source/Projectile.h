#include "Resources.h"
struct Projectile{
    static constexpr auto SPEED = 15.0f;
    static constexpr auto WIDTH = 7.0f;
    static constexpr auto HEIGHT = 50.0f;
   
    Vector2 position{0,0};
    float speed = SPEED;
    bool active = true;
    Projectile(Vector2 pos, float velocity = SPEED) noexcept : position(pos), speed(velocity){
        if(velocity < 0){ //firing up from the player, so displace spawn position to the top of the player
            position.y -= HEIGHT;
        }
    }

    float x() const noexcept{
        return position.x;
    }
    float y() const noexcept{
        return position.y;
    }

    Rectangle hitBox() const noexcept{
        return Rectangle{position.x, position.y, WIDTH, HEIGHT};
    }

    void Update() noexcept{
        position.y += speed;
        if(position.y < -HEIGHT || position.y > GetScreenHeightF() + HEIGHT){
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
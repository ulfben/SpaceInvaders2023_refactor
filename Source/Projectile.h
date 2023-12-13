#include "Resources.h"
struct Projectile{
    Vector2 position = {0,0};
    int speed = 15;
    bool active = true;
    EntityType type = {};
    Vector2 lineStart = {0, 0};
    Vector2 lineEnd = {0, 0};
    void Update() noexcept{
        position.y -= speed;        
        lineStart.y = position.y - 15;
        lineEnd.y = position.y + 15;
        lineStart.x = position.x;
        lineEnd.x = position.x;
        if(position.y < 0 || position.y > 1500){
            active = false;
        }
    }

    void Render(const Texture2D& texture) const noexcept{      
        DrawTexturePro(texture,
            {0,0,176,176,}, {position.x,position.y,50,50,}, 
            {25 , 25}, 0, WHITE
        );
    }
};
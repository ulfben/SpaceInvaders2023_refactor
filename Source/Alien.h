#include "Resources.h"
struct Alien{
    EntityType type = EntityType::ENEMY;
    Color color = WHITE;
    Vector2 position = {0, 0};
    int x = 0;
    int y = 0;
    float radius = 30;
    bool active = true;
    bool moveRight = true;
    int speed = 2;
    void Update() noexcept{
        int window_width = GetScreenWidth();
        if(moveRight){
            position.x += speed;
            if(position.x >= GetScreenWidth()){
                moveRight = false;
                position.y += 50;
            }
        } else{
            position.x -= speed;
            if(position.x <= 0){
                moveRight = true;
                position.y += 50;
            }
        }
    }

    void Render(const Texture2D& texture) const noexcept{      
        DrawTexturePro(texture,
            {0,0,352,352,}, {position.x, position.y, 100, 100}, 
            {50 , 50}, 0, WHITE
        );
    }
};
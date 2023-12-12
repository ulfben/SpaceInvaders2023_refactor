#include "Resources.h"
struct Wall{
    Vector2 position;
    Rectangle rec;
    bool active;
    Color color;
    int health = 50;
    int radius = 60;
    void Render(Texture2D texture) noexcept{
        DrawTexturePro(texture,
            {0,0,704,704,}, {position.x,position.y,200,200,}, 
            {100 , 100},0,WHITE
        );
        DrawText(TextFormat("%i", health), position.x - 21, position.y + 10, 40, RED);
    }

    void Update() noexcept{
        if(health < 1){
            active = false;
        }
    }
};
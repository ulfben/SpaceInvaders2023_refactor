#include "Resources.h"

struct Wall{
    static constexpr int RADIUS = 60;
    static constexpr int LABEL_OFFSET_X = -21; 
    static constexpr int LABEL_OFFSET_Y = 10; 
    static constexpr Rectangle SOURCE = {0,0,704,704};
    static constexpr Vector2 ORIGIN = {100 , 100};
    static constexpr float WIDTH = 200;
    static constexpr float HEIGHT = 200;
    Vector2 position;    
    int health = 50;
    bool active = true; 

    Wall(int x, int y) noexcept : position{toFloat(x), toFloat(y)}{};

    void Render(const Texture2D& texture) const noexcept{
        DrawTexturePro(texture,
            SOURCE, {position.x, position.y, WIDTH, HEIGHT}, 
            ORIGIN,0,WHITE
        );
        Vector2 label_pos{position.x + LABEL_OFFSET_X, position.y + LABEL_OFFSET_Y};
        DrawText(TextFormat("%i", health), label_pos, 40, RED);
    }

    void Update() noexcept{
        active = (health > 0);
    }
};
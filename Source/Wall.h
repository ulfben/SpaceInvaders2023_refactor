#pragma once
#include "UtilsRaylib.h"

struct Wall{
    static constexpr int WIDTH = 164; //TODO: these should be read from the sprite... 
    static constexpr int HEIGHT = 73;    
    Vector2 position;
    int health = 50;

    Wall(float x, float y) noexcept : position{x, y}{};

    float x() const noexcept{
        return position.x;
    }
    float y() const noexcept{
        return position.y;
    }

    Rectangle hitBox() const noexcept{
        return Rectangle{x(), y(), WIDTH, HEIGHT};
    }

    void Render(const Texture2D& tx) const noexcept{
        DrawTexture(tx, x(), y());
        const float label_offset = toFloat(tx.width) * 0.4f;
        const Vector2 label_pos{x() + label_offset, y() + tx.height / 2};
        DrawText(TextFormat("%i", health), label_pos, tx.height / 2, RED);
    }

    bool isAlive() const noexcept{
        return health > 0;
    }
};
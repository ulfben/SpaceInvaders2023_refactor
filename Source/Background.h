#include "Resources.h"
struct Star{
    Vector2 initPosition = {0, 0};
    Vector2 position = {0, 0};
    Color color = GRAY;
    float size = 0;
    void Update(float starOffset) noexcept{
        position.x = initPosition.x + starOffset;
        position.y = initPosition.y;
    }
    void Render() const noexcept{
        DrawCircle((int) position.x, (int) position.y, size, color);
    }
};
struct Background{
    std::vector<Star> Stars;
    void Initialize(int starAmount){
        for(int i = 0; i < starAmount; i++){
            Star newStar;
            newStar.initPosition.x = GetRandomValue(-150, GetScreenWidth() + 150);
            newStar.initPosition.y = GetRandomValue(0, GetScreenHeight());            
            newStar.color = SKYBLUE;
            newStar.size = GetRandomValue(1, 4) / 2;
            Stars.push_back(newStar);
        }
    }

    void Update(float offset) noexcept{
        for(int i = 0; i < Stars.size(); i++){
            Stars[i].Update(offset);
        }
    }

    void Render() const noexcept{
        for(int i = 0; i < Stars.size(); i++){
            Stars[i].Render();
        }
    }
};
#include "Resources.h"
#include <vector>
class Star{
    static constexpr auto COLOR = GRAY;        
    Vector2 position{};
    float size = 1.0f;
public:
    Star(int x, int y, int size) noexcept : position{toFloat(x), toFloat(y)}, size(toFloat(size)){};
      
    void Render(float offset) const noexcept{
        const float scrolling_offset = offset/size;
        DrawCircle(toInt<int>(position.x+scrolling_offset), toInt<int>(position.y), size, COLOR);
    }
};

class Background{
    static constexpr auto PARALLAX_DISTANCE = 85.0f; //ratio for how much the stars should move relative to the player's movement
    static constexpr auto MARGIN = 150;
    std::vector<Star> Stars;
    float scrolling_offset = 0;
public:
    explicit Background(unsigned starAmount){
        Stars.reserve(starAmount);
        const auto max_x = GetScreenWidth() + MARGIN;
        const auto max_y = GetScreenHeight();
        for(unsigned i = 0; i < starAmount; i++){
            const auto x = GetRandomValue(-MARGIN, max_x);
            const auto y = GetRandomValue(0, max_y);                
            const auto size = GetRandomValue(1, 3);
            Stars.emplace_back(x, y, size);
        }
    }

    void Update(float parallaxRatio) noexcept{                
        scrolling_offset = parallaxRatio * PARALLAX_DISTANCE * -1.0f;                
    }

    void Render() const noexcept{
        for(const auto& star : Stars){
            star.Render(scrolling_offset);
        }
    }
};
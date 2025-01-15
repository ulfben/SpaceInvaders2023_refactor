#pragma once
#include "UtilsRaylib.h"
#include <vector>

class Star{
    static constexpr auto COLOR = GRAY;        
    float _x = 0; 
    int _y = 0;
    float _size = 0.0f;
public:
    Star(int x, int y, float size) noexcept : _x(toFloat(x)), _y(y), _size(size){};
      
    void Render(float offset) const noexcept{
        const float scrolling_offset = offset/_size;        
        DrawCircle(floor<int>(_x+scrolling_offset), _y, _size, COLOR);
    }
};

class Background{
    static constexpr auto PARALLAX_DISTANCE = 85.0f; //ratio for how much the stars should move relative to the player's movement
    static constexpr auto MARGIN = 150;
    std::vector<Star> stars;
    float scrolling_offset = 0;
public:
    explicit Background(unsigned starAmount){
        stars.reserve(starAmount);
        const auto max_x = GetScreenWidth() + MARGIN;
        const auto max_y = GetScreenHeight();
        for(unsigned i = 0; i < starAmount; i++){
            const auto x = GetRandomValue(-MARGIN, max_x);
            const auto y = GetRandomValue(0, max_y);                
            const auto size = GetRandomValueF(1, 3);
            stars.emplace_back(x, y, size);
        }
    }

    void Update(float parallaxRatio) noexcept{                
        scrolling_offset = parallaxRatio * PARALLAX_DISTANCE * -1.0f;                
    }

    void Render() const noexcept{
        for(const auto& star : stars){
            star.Render(scrolling_offset);
        }
    }
};
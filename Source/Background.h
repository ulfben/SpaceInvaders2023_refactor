#pragma once
#include "UtilsRaylib.h"
#include <vector>
#include <cassert>

class Background{
    class Stars{
        static constexpr auto COLOR = GRAY;
        std::vector<float> _x;
        std::vector<int>   _y;
        std::vector<float> _radius;

    public:
        explicit Stars(unsigned count){
            _x.reserve(count);
            _y.reserve(count);
            _radius.reserve(count);
            const int max_x = GetScreenWidth();
            const int max_y = GetScreenHeight();
            for(unsigned i = 0; i < count; ++i){
                _x.push_back(toFloat(GetRandomValue(-150, max_x + 150)));
                _y.push_back(GetRandomValue(0, max_y));
                _radius.push_back(GetRandomValueF(1.0f, 3.0f));
            }
        }

        void Render(float offset) const noexcept{
            assert(_x.size() == _y.size() && _x.size() == _radius.size());
            const std::size_t count = _x.size();
            for(std::size_t i = 0; i < count; ++i){
                [[gsl::suppress(bounds.4)]] //index is garuantueed to be in range.
                const float parallax = offset / _radius[i]; //TODO: consider pre-computing inv_size = 1.0f / size to get rid of the division here.
                [[gsl::suppress(bounds.4)]]
                DrawCircle(floor<int>(_x[i] + parallax), _y[i], _radius[i], COLOR);
            }
        }
    };

    static constexpr auto PARALLAX_DISTANCE = 85.0f;
    static constexpr auto MARGIN = 150;
    Stars stars;
    float scrolling_offset = 0;

public:
    explicit Background(unsigned starAmount) : stars(starAmount){}

    void Update(float parallaxRatio) noexcept{                
        scrolling_offset = parallaxRatio * PARALLAX_DISTANCE * -1.0f;                
    }

    void Render() const noexcept{
        stars.Render(scrolling_offset);
    }
};
#pragma once
#include "Settings.h"
#include "AutoTexture.h"
#include "raylib.h"
#include "UtilsRaylib.h"
#include <algorithm> //for std::clamp
#include <span>
#include <string_view>
#include <vector>   

struct Animation{
    std::vector<AutoTexture> frames;
    size_t current = 0;
    unsigned displayTime = 15; //in ticks   
    unsigned tickCount = 0;

    explicit Animation(std::span<const std::string_view> paths){
        frames.reserve(paths.size());
        for(auto path : paths){ //by copy, string_view is a cheap value type
            frames.emplace_back(path);
        }
    }

    void update() noexcept{
        tickCount++;
        if(tickCount > displayTime){
            tickCount = 0;
            current = (current + 1) % frames.size();
        }
    }
    const Texture2D& currentFrame() const noexcept{
        [[gsl::suppress(bounds.4, justification: "Animation owns and guarantuees that the index is valid.")]]
        return frames[current].get();
    }
    float width() const noexcept{
        return toFloat(currentFrame().width);
    }
    float height() const noexcept{
        return toFloat(currentFrame().height);
    }
};

struct Player{
    static constexpr float SPEED = 7;
    static constexpr float WIDTH = 94;
    static constexpr float HEIGHT = 97;
    Animation animation{PlayerAnimationFiles};
    Vector2 position{GetScreenWidthF() / 2.0f, GetScreenHeightF() - HEIGHT};
    int lives = 3;     

    float x() const noexcept{
        return position.x;
    }
    float y() const noexcept{
        return position.y;
    }
    float width() const noexcept{
        return animation.width();
    }
    float height() const noexcept{
        return animation.height();
    }
    float right() const noexcept{
        return x() + width();
    }
    float left() const noexcept{
        return x();
    }
    float top() const noexcept{ 
        return y();
    }
    float centerX() const noexcept{
        return x() + animation.width() / 2.0f;
    }
    Vector2 gunPosition() const noexcept{
        return {centerX(), top() };
    }
    Rectangle hitbox() const noexcept{
        return {x(), y(), animation.width(), animation.height()};
    }

    void Update() noexcept{
        const auto direction = IsKeyDown(KEY_LEFT) ? -1.0f : (IsKeyDown(KEY_RIGHT) ? 1.0f : 0.0f);
        position.x += SPEED * direction;
        position.x = std::clamp(x(), 0.0f, GetScreenWidthF()-width());
        animation.update();
    }

    void Render() const noexcept{        
        DrawTexture(animation.currentFrame(), position);  
    }
};
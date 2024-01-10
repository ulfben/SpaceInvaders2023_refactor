#pragma once
#include "Resources.h"
#include <algorithm> //for std::clamp
#include <string_view>
#include <span>
#include <vector>   



struct Animation{
    std::vector<OwnTexture> frames;
    size_t frame = 0;
    unsigned displayTime = 15; //in ticks   
    unsigned tickCount = 0;

    explicit Animation(std::span<const std::string_view> paths){
        frames.reserve(paths.size());
        for(auto& path : paths){
            frames.emplace_back(path);
        }
    }

    void update() noexcept{
        tickCount++;
        if(tickCount > displayTime){
            tickCount = 0;
            frame = (frame + 1) % frames.size();
        }
    }
    const Texture2D& currentFrame() const noexcept{
        return frames[frame].get();
    }
};

struct Player{
    static constexpr float SPEED = 7;
    static constexpr float RADIUS = 50;
    Animation animation{PlayerAnimationFiles};
    Vector2 pos{GetScreenWidthF() / 2.0f, GetScreenHeightF() - RADIUS};
    int lives = 3;     

    float x() const noexcept{
        return pos.x;
    }
    float y() const noexcept{
        return pos.y;
    }
    Vector2 gunPosition() const noexcept{
        return {x(), y() - RADIUS};
    }

    void Update() noexcept{
        auto direction = IsKeyDown(KEY_LEFT) ? -1.0f : (IsKeyDown(KEY_RIGHT) ? 1.0f : 0.0f);
        pos.x += SPEED * direction;
        pos.x = std::clamp(x(), RADIUS, GetScreenWidthF() - RADIUS);
        animation.update();
    }

    void Render() const noexcept{        
        DrawTexture(animation.currentFrame(), x()-RADIUS, y()-RADIUS, WHITE);  
    }
};
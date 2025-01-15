#pragma once
#include "AutoTexture.h"
#include "UtilsRaylib.h"
#include <assert.h>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>   

class Animation{
    static constexpr size_t TICKS_PER_FRAME = 15;
    std::vector<AutoTexture> frames;
    size_t current = 0;    
    size_t tickCount = 0;

    static constexpr size_t advanceAndWrap(size_t i, size_t limit) noexcept{
        assert(limit > 0);
        return ++i % limit;
    }

public:
    explicit Animation(std::span<const std::string_view> paths){
        frames.reserve(paths.size());
        for(auto path : paths){
            frames.emplace_back(path);
        }
        if(frames.empty()){
            throw std::runtime_error("No frames in animation.");
        }
    }

    void update() noexcept{
        tickCount = advanceAndWrap(tickCount, TICKS_PER_FRAME);
        if(tickCount == 0){            
            current = advanceAndWrap(current, frameCount());
        }
    }
    const Texture2D& currentFrame() const noexcept{
        [[gsl::suppress(bounds.4)]]
        return frames[current].get();
    }
    size_t frameCount() const noexcept{
        return frames.size();
    }
    float width() const noexcept{
        return toFloat(currentFrame().width);
    }
    float height() const noexcept{
        return toFloat(currentFrame().height);
    }
};
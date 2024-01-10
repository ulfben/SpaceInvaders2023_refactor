#pragma once
#include "raylib.h"
#include <array>
#include <vector>
#include <string_view>
#include <stdexcept>
#include <format>
#include <cmath>
#include <type_traits>

using namespace std::literals::string_view_literals;
static constexpr unsigned STAR_COUNT = 600;
static constexpr unsigned WALL_COUNT = 5;
static constexpr unsigned WALL_DIST_FROM_BOTTOM = 250;
static constexpr int formationWidth = 2;
static constexpr int formationHeight = 2;
static constexpr int alienSpacing = 80;
static constexpr int formationX = 100;
static constexpr int formationY = 50;
static constexpr int ALIEN_SHOT_COOLDOWN = 60; //frames
static constexpr std::array PlayerAnimationFiles = {
    "./Assets/Ship1.png"sv,
    "./Assets/Ship2.png"sv,
    "./Assets/Ship3.png"sv
};


//TODO: utilities, move to separate header. 
static float toFloat(int value) noexcept{
    return static_cast<float>(value);
}

template <std::integral T>
static T toInt(float value) noexcept{
    return static_cast<T>(value);
}

template <std::integral T>
static T floor(float value) noexcept{
    return toInt<T>(std::floor(value));
}

static float GetScreenWidthF() noexcept{
    return static_cast<float>(GetScreenWidth());
}

static float GetScreenHeightF() noexcept{
    return static_cast<float>(GetScreenHeight());
}

template <typename T>
static float GetRandomValueF(T min, T max) noexcept{
    return toFloat(GetRandomValue(static_cast<int>(min), static_cast<int>(max)));
}

static void DrawTexture(const Texture2D& tex, float x, float y, Color tint = WHITE) noexcept{
    DrawTexture(tex, floor<int>(x), floor<int>(y), tint); 
}

static void DrawTexture(const Texture2D& tex, Vector2 pos, Color tint = WHITE) noexcept{
    DrawTexture(tex, pos.x, pos.y, tint); 
}

static void DrawText(std::string_view t, Vector2 pos, int fontsize, Color c) noexcept{
    DrawText(t.data(), floor<int>(pos.x), floor<int>(pos.y), fontsize, c);
}

class OwnTexture{
    Texture2D _tex;
public:
    explicit OwnTexture(std::string_view path){
        _tex = LoadTexture(path.data());
        if(_tex.id <= 0){
            throw(std::runtime_error(std::format("Unable to load texture: {}"sv, path)));
        }
    }

    OwnTexture(const OwnTexture& other) = delete;
    OwnTexture& operator=(const OwnTexture& other) = delete;

    OwnTexture(OwnTexture&& other) noexcept{
        std::swap(other._tex, _tex);
    };
    OwnTexture& operator=(OwnTexture&& other) noexcept{
        std::swap(other._tex, _tex);
        return *this;
    };

    ~OwnTexture() noexcept{
        UnloadTexture(_tex);
    }

    const Texture2D& get() const noexcept{
        return _tex;
    }
};

struct Resources{    
    OwnTexture alienTexture = OwnTexture("./Assets/Alien.png"sv);
    OwnTexture barrierTexture = OwnTexture("./Assets/Barrier.png"sv);
    OwnTexture laserTexture = OwnTexture("./Assets/Laser.png"sv);    
};

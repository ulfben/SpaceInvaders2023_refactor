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
static constexpr unsigned WALL_MARGIN = 125; //distance to edge of screen
static constexpr unsigned short ALIEN_COLUMNS = 1;
static constexpr unsigned short ALIEN_ROWS = 1;
static constexpr size_t ALIEN_COUNT = ALIEN_COLUMNS * ALIEN_ROWS;
static constexpr unsigned ALIEN_SPACING = 100;
static constexpr unsigned ALIEN_FORMATION_TOP = 50;
static constexpr int ALIEN_SHOT_COOLDOWN = 60; //frames
static constexpr int POINTS_PER_ALIEN = 100;
static constexpr size_t MAX_HIGHSCORE_ENTRIES = 5;
static constexpr std::string_view scoreFile = "./Assets/HighScores.txt"sv;
static constexpr std::array PlayerAnimationFiles = {
    "./Assets/Ship1.png"sv,
    "./Assets/Ship2.png"sv,
    "./Assets/Ship3.png"sv
};

//TODO: utilities, move to separate header. 

//write a narrowing cast function
template <typename T, typename U>
static T narrow_cast(U&& u) noexcept{
    return static_cast<T>(std::forward<U>(u));
}

static char toChar(int i) noexcept{
    if(i < std::numeric_limits<char>::min() || i > std::numeric_limits<char>::max()){        
       return '\0';
    }
    return narrow_cast<char>(i);
}

template <std::integral T>
static float toFloat(T value) noexcept{
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

//assumes textures are drawn from their center
static Rectangle MakeCollisionHull(const Texture2D& tex, Vector2 pos) noexcept{        
    const auto width = toFloat(tex.width);
    const auto height = toFloat(tex.height);    
    const auto left = pos.x - (width * 0.5f);
    const auto top = pos.y - (height * 0.5f);
    return Rectangle{left, top, width, height};
}

struct LoadTextureError : public std::runtime_error{
    explicit LoadTextureError(std::string_view msg) : std::runtime_error(msg.data()){}
};

class OwnTexture{
    Texture2D _tex;
public:
    explicit OwnTexture(std::string_view path){
        _tex = LoadTexture(path.data());
        if(_tex.id <= 0){
            throw(LoadTextureError(std::format("Unable to load texture: {}"sv, path)));
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

    float width() const noexcept{
        return static_cast<float>(_tex.width);
    }
    float height() const noexcept{
        return static_cast<float>(_tex.height);
    }
    Rectangle rect() const noexcept{
        return Rectangle{0, 0, width(), height()};
    }
};

struct Resources{    
    OwnTexture alienTexture = OwnTexture{"./Assets/Alien.png"sv};
    OwnTexture barrierTexture = OwnTexture("./Assets/Barrier.png"sv);
    OwnTexture laserTexture = OwnTexture("./Assets/Laser.png"sv);    
};

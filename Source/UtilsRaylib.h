#pragma once
#include <codeanalysis\warnings.h>
#pragma warning(push)
#pragma warning(disable:ALL_CODE_ANALYSIS_WARNINGS)
#include "raylib.h"
#pragma warning(pop)
#include <cmath> //for std::floor
#include <concepts>
#include <limits>
#include <string>
#include <string_view>
#include <stdexcept>

/*A few utility functions, followed by some useful overloads for Raylib functions*/

struct LoadTextureError : public std::runtime_error{
    explicit LoadTextureError(std::string_view msg) : std::runtime_error(msg.data()){}
};

template <typename T, typename U>
constexpr T narrow_cast(U&& u) noexcept{
    return static_cast<T>(std::forward<U>(u));
}

constexpr char toChar(int i) noexcept{
    if(i < std::numeric_limits<char>::min() || i > std::numeric_limits<char>::max()){        
       return '\0';
    }
    return narrow_cast<char>(i);
}

template <std::integral T>
constexpr float toFloat(T value) noexcept{
    return static_cast<float>(value);
}

constexpr int toInt(float value) noexcept{
    return narrow_cast<int>(value);
}

template <std::integral T>
constexpr T floor(float value) noexcept{
    return narrow_cast<T>(std::floor(value));
}

inline float GetScreenWidthF() noexcept{
    return toFloat(GetScreenWidth());
}

inline float GetScreenHeightF() noexcept{
    return toFloat(GetScreenHeight());
}

template <typename T>
inline float GetRandomValueF(T min, T max) noexcept{
    return toFloat(GetRandomValue(narrow_cast<int>(min), narrow_cast<int>(max)));
}

inline void DrawTexture(const Texture2D& tex, float x, float y, Color tint = WHITE) noexcept{
    DrawTexture(tex, floor<int>(x), floor<int>(y), tint); 
}

inline void DrawTexture(const Texture2D& tex, Vector2 pos, Color tint = WHITE) noexcept{
    DrawTexture(tex, pos.x, pos.y, tint); 
}

inline void DrawText(std::string_view t, float x, float y, int fontsize, Color c) noexcept{
    DrawText(t.data(), floor<int>(x), floor<int>(y), fontsize, c);
}

inline void DrawText(std::string_view t, Vector2 pos, int fontsize, Color c) noexcept{
    DrawText(t, pos.x, pos.y, fontsize, c);
}
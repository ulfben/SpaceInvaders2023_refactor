#pragma once
#include "raylib.h"
#include <cmath> //for std::floor
#include <concepts>
#include <limits>
#include <string>
#include <string_view>


/* A macro to let us suppress the C++ Core Guideline warnings, in a consistent way across compilers.
* The expected syntax is: 
*    [[gsl::suppress(bounds.4)]], or 
*    [[gsl::suppress(bounds.4, justification: "Some reason.")]]
* but clang expects the argument(s) in quotes, whereas mscv expects them unquoted. 
* Untill this is sorted out, we'll use our own GSL_SUPPRESS(); 
*/
#if defined(__clang__)
    #define GSL_STRINGIFY(x) #x
    #define GSL_SUPPRESS_1(x) GSL_STRINGIFY(x)
    #define GSL_SUPPRESS_2(x, y) GSL_STRINGIFY(x), GSL_STRINGIFY(y)
    #define GSL_SELECT_MACRO(_1,_2,NAME,...) NAME
    #define GSL_SUPPRESS(...) [[gsl::suppress(GSL_SELECT_MACRO(__VA_ARGS__, GSL_SUPPRESS_2, GSL_SUPPRESS_1)(__VA_ARGS__))]]
#else
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
    #define GSL_SUPPRESS(...) [[gsl::suppress(__VA_ARGS__)]]
#else
    #define GSL_SUPPRESS(...)
#endif // _MSC_VER
#endif // __clang__



/*A few utility functions, followed by some useful overloads for Raylib functions*/

template <typename T, typename U>
inline T narrow_cast(U&& u) noexcept{
    return static_cast<T>(std::forward<U>(u));
}

inline char toChar(int i) noexcept{
    if(i < std::numeric_limits<char>::min() || i > std::numeric_limits<char>::max()){        
       return '\0';
    }
    return narrow_cast<char>(i);
}

template <std::integral T>
inline float toFloat(T value) noexcept{
    return static_cast<float>(value);
}

inline int toInt(float value) noexcept{
    return narrow_cast<int>(value);
}

template <std::integral T>
inline T floor(float value) noexcept{
    return narrow_cast<T>(std::floor(value));
}

inline float GetScreenWidthF() noexcept{
    return static_cast<float>(GetScreenWidth());
}

inline float GetScreenHeightF() noexcept{
    return static_cast<float>(GetScreenHeight());
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
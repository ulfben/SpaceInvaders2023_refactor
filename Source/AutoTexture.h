#pragma once
#include "UtilsRaylib.h"
#include <string>
#include <string_view>

class AutoTexture{
    Texture2D _tex{};
public:
    explicit AutoTexture(std::string_view path){
        const std::string tmp(path); 
        _tex = LoadTexture(tmp.c_str());
        if(_tex.id <= 0){
            throw LoadTextureError(path);
        }
    }

    AutoTexture(const AutoTexture& other) = delete;
    AutoTexture& operator=(const AutoTexture& other) = delete;

    AutoTexture(AutoTexture&& other) noexcept 
        : _tex(std::exchange(other._tex, Texture2D{}))
    {}

    AutoTexture& operator=(AutoTexture&& other) noexcept{
        if(this != &other){
            if(_tex.id){ UnloadTexture(_tex); }
            _tex = std::exchange(other._tex, Texture2D{});
        }
        return *this;
    };

    ~AutoTexture() noexcept{
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
        return Rectangle{0.0f, 0.0f, width(), height()};
    }
};
#pragma once
#include "raylib.h"
#include <string_view>
#include <stdexcept>
#include <format>
using namespace std::literals::string_view_literals;

struct LoadTextureError : public std::runtime_error{
    explicit LoadTextureError(std::string_view msg) : std::runtime_error(msg.data()){}
};

class AutoTexture{
    Texture2D _tex;
public:
    explicit AutoTexture(std::string_view path){
        _tex = LoadTexture(path.data());
        if(_tex.id <= 0){
            throw(LoadTextureError(std::format("Unable to load texture: {}"sv, path)));
        }
    }

    AutoTexture(const AutoTexture& other) = delete;
    AutoTexture& operator=(const AutoTexture& other) = delete;

    AutoTexture(AutoTexture&& other) noexcept{
        std::swap(other._tex, _tex);
    };

    AutoTexture& operator=(AutoTexture&& other) noexcept{
        std::swap(other._tex, _tex);
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
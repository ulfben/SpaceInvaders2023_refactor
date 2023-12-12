#pragma once
#include "raylib.h"
#include <vector>
#include <string_view>
#include <stdexcept>
#include <format>
using namespace std::literals::string_view_literals;

struct OwnTexture {
    Texture2D _tex;
    explicit OwnTexture(std::string_view path) {
        _tex = LoadTexture(path.data());
        if (_tex.id <= 0) {
            throw(std::runtime_error(std::format("Unable to load texture: {}"sv, path)));
        }
    }
    ~OwnTexture() {
        UnloadTexture(_tex);
    }

    const Texture2D& get() const noexcept {
        return _tex;
    }
};

struct Resources {
    std::vector<OwnTexture> shipTextures{
          OwnTexture("./Assets/Ship1.png"sv),
          OwnTexture("./Assets/Ship2.png"sv),
          OwnTexture("./Assets/Ship3.png"sv)
    };
    OwnTexture alienTexture = OwnTexture("./Assets/Alien.png"sv);
    OwnTexture barrierTexture = OwnTexture("./Assets/Barrier.png"sv);
    OwnTexture laserTexture = OwnTexture("./Assets/Laser.png"sv);
};
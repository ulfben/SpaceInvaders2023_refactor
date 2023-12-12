#pragma once
#include "raylib.h"
#include <vector>
#include <string_view>
#include <stdexcept>
#include <format>
using namespace std::literals::string_view_literals;

enum struct EntityType{ //TODO: temporary re-location. Will delete.
    PLAYER,
    ENEMY,
    PLAYER_PROJECTILE,
    ENEMY_PROJECTILE
};

class OwnTexture {    
    Texture2D _tex;
public:
    explicit OwnTexture(std::string_view path) {        
         _tex = LoadTexture(path.data());
        if (_tex.id <= 0) {
            throw(std::runtime_error(std::format("Unable to load texture: {}"sv, path)));
        }
    }
    
    OwnTexture(const OwnTexture& other) = delete; 
    OwnTexture& operator=(const OwnTexture& other) = delete;     
    
    OwnTexture(OwnTexture&& other) noexcept {
        std::swap(other._tex, _tex);
    };
    OwnTexture& operator=(OwnTexture&& other) noexcept {
        std::swap(other._tex, _tex);
        return *this;
    };

    ~OwnTexture() noexcept {
        UnloadTexture(_tex);
    }

    const Texture2D& get() const noexcept {
        return _tex;
    }
};

struct Resources {
    std::vector<OwnTexture> shipTextures;    
    Resources() {
        shipTextures.reserve(3);
        shipTextures.emplace_back("./Assets/Ship1.png"sv);
        shipTextures.emplace_back("./Assets/Ship2.png"sv);
        shipTextures.emplace_back("./Assets/Ship3.png"sv);
    }    
    OwnTexture alienTexture = OwnTexture("./Assets/Alien.png"sv);
    OwnTexture barrierTexture = OwnTexture("./Assets/Barrier.png"sv);
    OwnTexture laserTexture = OwnTexture("./Assets/Laser.png"sv);
};
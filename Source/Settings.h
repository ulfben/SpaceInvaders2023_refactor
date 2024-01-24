#pragma once
#include <string>
#include <string_view>
#include <array>
using namespace std::literals::string_view_literals;

constexpr std::string_view TITLE = "Space Invaders"sv;
constexpr unsigned WINDOW_WIDTH = 1280;
constexpr unsigned WINDOW_HEIGHT = 720;
constexpr unsigned FPS = 60;

static constexpr unsigned STAR_COUNT = 600;
static constexpr unsigned WALL_COUNT = 5;
static constexpr unsigned WALL_DIST_FROM_BOTTOM = 250;
static constexpr unsigned WALL_MARGIN = 125; //distance to edge of screen
static constexpr unsigned short ALIEN_COLUMNS = 6;
static constexpr unsigned short ALIEN_ROWS = 4;
static constexpr size_t ALIEN_COUNT = ALIEN_COLUMNS * ALIEN_ROWS;
static constexpr unsigned ALIEN_SPACING = 100;
static constexpr unsigned ALIEN_FORMATION_TOP = 50;
static constexpr int ALIEN_SHOT_COOLDOWN = 30; //frames
static constexpr int POINTS_PER_ALIEN = 100;
static constexpr size_t MAX_HIGHSCORE_ENTRIES = 5;
static constexpr std::string_view scoreFile = "./Assets/HighScores.txt"sv;
static constexpr std::array PlayerAnimationFiles = {
    "./Assets/Ship1.png"sv,
    "./Assets/Ship2.png"sv,
    "./Assets/Ship3.png"sv
};
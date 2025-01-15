#include "EndScreen.h"
#include "Settings.h"
#include "StartScreen.h"
#include "State.h"
#include "UtilsRaylib.h"
#include <algorithm>
#include <format>
#include <iosfwd>
#include <iterator>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
static constexpr int SCORE_RECORDED = -1;

EndScreen::EndScreen(int score){
    current.score = score;
    current.name.reserve(MAX_INPUT_SIZE);
    loadScores(scoreFile);
}

std::unique_ptr<State> EndScreen::update() {
    if(IsKeyReleased(KEY_ENTER) && !isNewHighscore()){
        return std::make_unique<StartScreen>();
    }
    SetMouseCursor(mouseOnTextfield() ? MOUSE_CURSOR_IBEAM : MOUSE_CURSOR_DEFAULT);
    handleTextEntry();
    if(isNewHighscore() && isEntryComplete() && IsKeyReleased(KEY_ENTER)){
        addNewScore();
        saveScores(scoreFile);
    }
    return nullptr;
}

void EndScreen::render() const noexcept{
    drawTable();
    const bool newHighscore = isNewHighscore();
    DrawText(newHighscore ? "NEW HIGHSCORE!" : "PRESS ENTER TO CONTINUE",
        600, newHighscore ? 300 : 200, newHighscore ? 60 : 40, YELLOW);

    if(newHighscore){
        DrawText("PLACE MOUSE OVER INPUT BOX!", 600, 400, 20, YELLOW);
        drawTextBox();
        DrawText(current.name, textBox.x + 5.0f, textBox.y + 8.0f, 40, MAROON);
        if(isEntryComplete()){
            DrawText("PRESS ENTER TO CONTINUE", 600, 800, 40, YELLOW);
        }
    }
}

bool EndScreen::isNewHighscore() const noexcept{
    return current.score != SCORE_RECORDED && current.score > table.back().score;
}

bool EndScreen::mouseOnTextfield() const noexcept{
    return CheckCollisionPointRec(GetMousePosition(), textBox);
}

void EndScreen::handleTextEntry() noexcept{
    for(int key = GetCharPressed(); key > 0; key = GetCharPressed()){
        const char c = toChar(key);
        if(c > 31 && c < 127 && !isEntryComplete()){
            current.name.push_back(c);
        }
    }
    if(IsKeyPressed(KEY_BACKSPACE) && !current.name.empty()){
        current.name.pop_back();
    }
}
bool EndScreen::isEntryComplete() const noexcept{
    return current.name.size() == MAX_INPUT_SIZE;
}
void EndScreen::sortTable() noexcept{
    std::ranges::sort(table, [](const auto& a, const auto& b){
        return a.score > b.score;
        });
}
void EndScreen::addNewScore() noexcept {
    try{
        table.push_back(current);
    }catch(...){/*swallow exception. we can live with unsaved highscore.*/}
    sortTable();
    while(table.size() > MAX_HIGHSCORE_ENTRIES){
        table.pop_back();
    }
    current.score = SCORE_RECORDED;
}


void EndScreen::drawTable() const noexcept{
    DrawText("LEADERBOARD", 50, 100, 40, YELLOW);
    int yOffset = 140;
    constexpr int yOffsetStep = 40;
    for(const auto& line : table){
        DrawText(line.name.c_str(), 50, yOffset, 40, YELLOW);
        DrawText(TextFormat("%i", line.score), 350, yOffset, 40, YELLOW);
        yOffset += yOffsetStep;
    }
}

void EndScreen::drawTextBox() const noexcept{
    DrawRectangleRec(textBox, LIGHTGRAY);    
    const auto color = mouseOnTextfield() ? RED : DARKGRAY;
    DrawRectangleLinesEx(textBox, 1, color);
}

void EndScreen::loadScores(std::string_view path){
    std::ifstream file(path.data());
    if(!file.is_open()){
        return;
    }
    std::string line;
    table.reserve(MAX_HIGHSCORE_ENTRIES+1); //+1 for current score, avoids re-allocation in addNewScore()
    while(std::getline(file, line)){
        const auto pos = line.find(',');
        if(pos == std::string::npos){
            continue;
        }
        const auto name = line.substr(0, pos);
        const auto score = std::stoi(line.substr(pos + 1));
        table.emplace_back(name, score);
    }
    file.close();
    sortTable();
}

void EndScreen::saveScores(std::string_view path) const {
    std::ofstream file(path.data());
    if(!file.is_open()){
        return;
    }
    const auto iter = std::ostream_iterator<char>(file);
    for(const auto& entry : table){
        std::format_to(iter, "{},{}\n", entry.name, entry.score);
    }
}
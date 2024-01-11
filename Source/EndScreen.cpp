#include "State.h"
#include "Resources.h"
#include <format>
#include <algorithm>
#include <fstream>

bool EndScreen::isNewHighscore() const noexcept{
    return current.score > highscores.back().score;
}

EndScreen::EndScreen(int score){
    current.score = score;
    current.name.reserve(MAX_INPUT_SIZE);
    loadScores(scoreFile);
}

bool EndScreen::mouseOnTextfield() const noexcept{
    return CheckCollisionPointRec(GetMousePosition(), textBox);
}

void EndScreen::doTextEntry() noexcept{
    for(int key = GetCharPressed(); key > 0; key = GetCharPressed()){
        const char c = toChar(key);
        if(c > 31 && c < 126 && !isEntryComplete()){
            name.push_back(c);
        }
    }
    if(IsKeyPressed(KEY_BACKSPACE) && !name.empty()){
        name.pop_back();
    }
}
bool EndScreen::isEntryComplete() const noexcept{
    return name.size() == MAX_INPUT_SIZE;
}
void EndScreen::sortTable() noexcept{
    std::ranges::sort(highscores, [](const auto& a, const auto& b){
        return a.score > b.score;
    });
}
void EndScreen::addNewScore() {   
    if(highscores.size() == MAX_HIGHSCORE_ENTRIES){
        highscores.pop_back();
    }    
    highscores.push_back(current);
    sortTable();
}
std::unique_ptr<State> EndScreen::update() noexcept{
    if(IsKeyReleased(KEY_ENTER) && !isNewHighscore()){
        return std::make_unique<StartScreen>();
    }
    SetMouseCursor(mouseOnTextfield() ? MOUSE_CURSOR_IBEAM : MOUSE_CURSOR_DEFAULT);
    doTextEntry();
    if(isEntryComplete() && IsKeyReleased(KEY_ENTER)){
        addNewScore(); //TODO: double check that we have a new highscore´. and fix the noexcept specifiers
        saveScores(scoreFile);
    }
    return nullptr;
}

void EndScreen::render() const noexcept{
    if(!isNewHighscore()){
        DrawText("PRESS ENTER TO CONTINUE", 600, 200, 40, YELLOW);
        DrawText("LEADERBOARD", 50, 100, 40, YELLOW);
        for(int i = 0; i < highscores.size(); i++){
            DrawText(highscores[i].name.c_str(), 50, 140 + (i * 40), 40, YELLOW);
            DrawText(TextFormat("%i", highscores[i].score), 350, 140 + (i * 40), 40, YELLOW);
        }
        return;
    }
    DrawText("NEW HIGHSCORE!", 600, 300, 60, YELLOW);
    DrawText("PLACE MOUSE OVER INPUT BOX!", 600, 400, 20, YELLOW);
    DrawRectangleRec(textBox, LIGHTGRAY);
    const auto x = toInt<int>(textBox.x);
    const auto y = toInt<int>(textBox.y);
    const auto color = mouseOnTextfield() ? RED : DARKGRAY;
    DrawRectangleLines(x, y, toInt<int>(textBox.width), toInt<int>(textBox.height), color);
    DrawText(name.c_str(), x + 5, y + 8, 40, MAROON);
    if(isEntryComplete()){
        DrawText("PRESS ENTER TO CONTINUE", 600, 800, 40, YELLOW);
    }
}

void EndScreen::loadScores(std::string_view path){
    std::ifstream file(path.data());
    if(!file.is_open()){
        return;
    }
    std::string line;
    while(std::getline(file, line)){
        const auto pos = line.find(',');
        if(pos == std::string::npos){
            continue;
        }
        const auto name = line.substr(0, pos);        
        const auto score = std::stoi(line.substr(pos + 1));
        highscores.emplace_back(name, score);
    }
    file.close();
    sortTable();    
}

void EndScreen::saveScores(std::string_view path) noexcept{
    std::ofstream file(path.data());
    if(!file.is_open()){
        return;
    }
    sortTable();
    const auto iter = std::ostream_iterator<char>(file);
    for(const auto& entry : highscores){        
        std::format_to(iter, "{},{}\n", entry.name, entry.score);        
    }    
} 
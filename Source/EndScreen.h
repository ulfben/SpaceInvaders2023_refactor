#pragma once
#include "UtilsRaylib.h"
#include "State.h"
#include <memory>
#include <string>
#include <vector>
class EndScreen : public State{    
public:
    explicit EndScreen(int score);    
    std::unique_ptr<State> update() noexcept(false) override;
    void render() const noexcept override;

private: 
    static const size_t MAX_INPUT_SIZE = 3;
    void loadScores(std::string_view path);
    void addNewScore() noexcept;
    void saveScores(std::string_view path) const;   
    bool mouseOnTextfield() const noexcept;
    bool isNewHighscore() const noexcept;
    bool isEntryComplete() const noexcept;
    void handleTextEntry() noexcept;
    void sortTable() noexcept;
    void drawTable() const noexcept;
    void drawTextBox() const noexcept;
    struct ScoreEntry{
        std::string name{""};
        int score{0};
    };
    ScoreEntry current;
    std::vector<ScoreEntry> table; 
    Rectangle textBox = {600, 500, 225, 50};    
};
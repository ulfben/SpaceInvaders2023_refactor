#include "game.h"
#include <cassert>
#include <cmath>
#include <vector>
#include <span>
#include <algorithm>
struct Draw{
    Draw() noexcept{
        BeginDrawing();
        ClearBackground(BLACK);
    }
    ~Draw() noexcept{
        EndDrawing();
    }
};


void Game::Update(){
    State* next = state->update();
    if(next){
        //state->exit();
        state.reset(next);
    }
    /*switch(gameState){
    case State::ENDSCREEN:
        if(IsKeyReleased(KEY_ENTER) && !newHighScore){
            //save leaderboard
            return new Startscreen();
        }
        if(newHighScore){
            mouseOnText = (CheckCollisionPointRec(GetMousePosition(), textBox));
            if(mouseOnText){
                SetMouseCursor(MOUSE_CURSOR_IBEAM);
                int key = GetCharPressed();
                while(key > 0){
                    if((key >= 32) && (key <= 125) && (letterCount < 9)){
                        name[letterCount] = (char) key;
                        name[letterCount + 1] = '\0';
                        letterCount++;
                    }
                    key = GetCharPressed();
                }

                if(IsKeyPressed(KEY_BACKSPACE)){
                    letterCount--;
                    if(letterCount < 0) letterCount = 0;
                    name[letterCount] = '\0';
                }
            } else{
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            }

            if(mouseOnText){
                framesCounter++;
            } else{
                framesCounter = 0;
            }
            if(letterCount > 0 && letterCount < 9 && IsKeyReleased(KEY_ENTER)){
                std::string nameEntry(name);
                InsertNewHighScore(nameEntry);
                newHighScore = false;
            }
        }
        break;
    default:
        assert(false && "Game::update() invalid state reached");
        break;
    }*/
}

void Game::Render(){
    Draw d{};
    state->render();
    //const auto i = player.activeTexture;
    //switch(gameState){

    //case State::ENDSCREEN:
    //    if(newHighScore){
    //        DrawText("NEW HIGHSCORE!", 600, 300, 60, YELLOW);
    //        DrawText("PLACE MOUSE OVER INPUT BOX!", 600, 400, 20, YELLOW);
    //        DrawRectangleRec(textBox, LIGHTGRAY);
    //        const auto x = toInt<int>(textBox.x);
    //        const auto y = toInt<int>(textBox.y);
    //        const auto color = (mouseOnText) ? RED : DARKGRAY;
    //        DrawRectangleLines(x, y, (int) textBox.width, (int) textBox.height, color);
    //        DrawText(name, x + 5, y + 8, 40, MAROON);
    //        DrawText(TextFormat("INPUT CHARS: %i/%i", letterCount, 8), 600, 600, 20, YELLOW);

    //        if(mouseOnText){
    //            if(letterCount < 9){
    //                if(((framesCounter / 20) % 2) == 0){ //blinking caret position 
    //                    DrawText("_", (int) textBox.x + 8 + MeasureText(name, 40), (int) textBox.y + 12, 40, MAROON);
    //                }
    //            } else{
    //                DrawText("Press BACKSPACE to delete chars...", 600, 650, 20, YELLOW);
    //            }
    //        }
    //        if(letterCount > 0 && letterCount < 9){
    //            DrawText("PRESS ENTER TO CONTINUE", 600, 800, 40, YELLOW);
    //        }
    //    } else{
    //        // If no highscore or name is entered, show scoreboard and call it a day
    //        DrawText("PRESS ENTER TO CONTINUE", 600, 200, 40, YELLOW);
    //        DrawText("LEADERBOARD", 50, 100, 40, YELLOW);
    //        for(int i = 0; i < Leaderboard.size(); i++){
    //            DrawText(Leaderboard[i].name.c_str(), 50, 140 + (i * 40), 40, YELLOW);
    //            DrawText(TextFormat("%i", Leaderboard[i].score), 350, 140 + (i * 40), 40, YELLOW);
    //        }
    //    }
    //    break;
    //default:
    //    assert(false && "Game::render() invalid state reached");
    //    break;
    //}
}



bool Game::CheckNewHighScore(){ //TODO: remove 
    return false;
    //return (score > Leaderboard[4].score);
}

void Game::InsertNewHighScore(const std::string& name){
    //Leaderboard.emplace_back(name, score);
    //std::ranges::sort(Leaderboard, [](const auto& a, const auto& b){
    //    return a.score < b.score;
    //    });
    //Leaderboard.pop_back();
}

void Game::LoadLeaderboard(){
  //TODO: implement leaderboard loading
}

void Game::SaveLeaderboard(){
  //TODO: implement leaderboard saving
}
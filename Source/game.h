#pragma once
#include "Settings.h"
#include "State.h"
#include "StartScreen.h"
#include "Window.h"
#include <memory>
#include <string_view>

struct Game{  
    Window w{TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, FPS};        
    std::unique_ptr<State> state = std::make_unique<StartScreen>();    
    void run();
    
private:
    void Update();
    void Render() const noexcept;   
};
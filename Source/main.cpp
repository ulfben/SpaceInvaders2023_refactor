#include "Game.h"
#include <exception>
#include <print>
#include <stdexcept>

int main(void) {
    try {        
        Game game{};
        game.run();
    }    
    catch (const std::runtime_error& e) {
        std::println("{}", e.what());
    }
    catch(const std::bad_alloc& e){
        std::println("Out of memory: {}", e.what());
    }
    catch (...) {
        std::println("Unkown error!");
    }
    return 0;
}
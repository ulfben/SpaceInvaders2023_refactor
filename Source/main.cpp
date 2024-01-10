#include <print>
#include <stdexcept>
#include "game.h"

int main(void) {
    try {        
        Game game{};
        game.run();
    }  
    catch (const std::runtime_error& e) {
        std::println("{}", e.what());
    }
    catch (...) {
        std::println("Unkown error!");
    }
    return 0;
}
#include "Game.h"
#include "UtilsRaylib.h"
#include <exception>
#include <print>
#include <stdexcept>

int main(void) {
    try {        
        Game game{};
        game.run();
    }    
    catch(const LoadTextureError& e){
        std::println("Texture unavailable: {}. Exiting.", e.what());
    }    
    catch(const std::bad_alloc& e){
        std::println("Out of memory: {}. Exiting.", e.what());
    }
    catch (const std::runtime_error& e) {
        std::println("{}", e.what());
    }
    catch (...) {
        std::println("Unkown error! Exiting.");
    }
    return 0;
}
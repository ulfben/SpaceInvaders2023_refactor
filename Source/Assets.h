#pragma once
#include "AutoTexture.h"

struct Assets{   
    //Note: the Player class owns its own assets, so it's not included here.
    AutoTexture alien = AutoTexture{"./Assets/Alien.png"sv};
    AutoTexture wall = AutoTexture{"./Assets/Barrier.png"sv};
    AutoTexture beam = AutoTexture{"./Assets/Laser.png"sv};
};
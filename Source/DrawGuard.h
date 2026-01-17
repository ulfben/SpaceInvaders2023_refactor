#pragma once
#include "UtilsRaylib.h"
struct DrawGuard{
    DrawGuard(Color background) noexcept{
        BeginDrawing();
        ClearBackground(background);
    }
    ~DrawGuard() noexcept{
        EndDrawing();
    }
    DrawGuard(const DrawGuard&) = delete;
    DrawGuard& operator=(const DrawGuard&) = delete;
    DrawGuard(DrawGuard&&) = delete;
    DrawGuard& operator=(DrawGuard&&) = delete;
};
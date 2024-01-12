#pragma once
#include "raylib.h"
struct DrawGuard{
    DrawGuard() noexcept{
        BeginDrawing();
        ClearBackground(BLACK);
    }
    ~DrawGuard() noexcept{
        EndDrawing();
    }
    DrawGuard(const DrawGuard&) = delete;
    DrawGuard& operator=(const DrawGuard&) = delete;
    DrawGuard(DrawGuard&&) = delete;
    DrawGuard& operator=(DrawGuard&&) = delete;
};
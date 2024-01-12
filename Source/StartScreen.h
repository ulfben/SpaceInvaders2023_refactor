#pragma once
#include "State.h"
#include <memory>
struct StartScreen : public State{
    std::unique_ptr<State> update() noexcept(false) override;
    void render() const noexcept override;
};
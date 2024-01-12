#pragma once
#include <memory>

//State::update() is noexcept(false) because it needs to allocate new State objects, and their ctors needs to throw.
//  A possible solution is to pre-allocate all possible states and provide a reset()-function.
class State{
public:    
    virtual std::unique_ptr<State> update() noexcept(false) = 0;
    virtual void render() const noexcept = 0;
    virtual ~State() noexcept = default;
};
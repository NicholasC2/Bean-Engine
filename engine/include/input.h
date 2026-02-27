#pragma once
#include <unordered_map>
#include <array>
#include <cstddef>

namespace Input {

enum class Action {
    Jump,
    Interact,
    Sneak,
    MoveForward,
    MoveBackward,
    MoveLeft,
    MoveRight,
    COUNT
};

constexpr size_t ActionCount = static_cast<size_t>(Action::COUNT);

using KeyCode = int;

void setDefaultBindings();

void poll();

void resetFrame();

bool isDown(Action action);
bool isPressed(Action action);
bool isReleased(Action action);

}
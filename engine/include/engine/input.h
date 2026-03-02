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

extern bool prevMouseState;

constexpr size_t ActionCount = static_cast<size_t>(Action::COUNT);

using KeyCode = int;

void setDefaultBindings();

static bool mouseDown;
static bool mousePressed;
static bool mouseReleased;

void getMousePos(float* x, float* y);
bool getMouseState();

void poll();

void resetFrame();

bool isDown(Action action);
bool isPressed(Action action);
bool isReleased(Action action);

bool isMouseDown();
bool isMousePressed();
bool isMouseReleased();

}
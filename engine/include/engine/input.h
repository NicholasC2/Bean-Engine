#pragma once

#include <SDL3/SDL.h>

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

using KeyCode = SDL_Scancode;

void setDefaultBindings();

void resetFrame();

void getMousePos(float* x, float* y);

bool getMouseState();

void poll();

bool isDown(Action action);
bool isPressed(Action action);
bool isReleased(Action action);

bool isMouseDown();
bool isMousePressed();
bool isMouseReleased();

}
#include "engine/input.h"
#include <unordered_map>
#include <array>
#include <SDL3/SDL.h>
#include <algorithm>
#include <iostream>

namespace Input {

bool prevMouseState = false;

constexpr size_t ActionCount = static_cast<size_t>(Action::COUNT);

static std::array<bool, ActionCount> actionDown{};
static std::array<bool, ActionCount> actionPressed{};
static std::array<bool, ActionCount> actionReleased{};

using KeyCode = SDL_Scancode;

static std::unordered_map<Action, KeyCode> keyBindings;

void setDefaultBindings() {
    keyBindings = {
        { Action::Jump, SDL_SCANCODE_SPACE },
        { Action::Interact, SDL_SCANCODE_E },
        { Action::Sneak, SDL_SCANCODE_LSHIFT },
        { Action::MoveForward, SDL_SCANCODE_W },
        { Action::MoveBackward, SDL_SCANCODE_S },
        { Action::MoveLeft, SDL_SCANCODE_A },
        { Action::MoveRight, SDL_SCANCODE_D }
    };
}

void resetFrame() {
    std::fill(actionPressed.begin(), actionPressed.end(), false);
    std::fill(actionReleased.begin(), actionReleased.end(), false);
}

static bool mouseDown = false;
static bool mousePressed = false;
static bool mouseReleased = false;

void getMousePos(float* x, float* y) {
    float mx, my;
    SDL_GetMouseState(&mx, &my);
    if (x) *x = static_cast<float>(mx);
    if (y) *y = static_cast<float>(my);
}

bool getMouseState() {
    SDL_MouseButtonFlags mb_flags = SDL_GetMouseState(NULL, NULL);
    return (mb_flags & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)) != 0;
}


void poll() {
    resetFrame();

    SDL_PumpEvents();

    int numKeys;
    const bool* state = SDL_GetKeyboardState(&numKeys);

    for (auto& pair : keyBindings) {
        Action action = pair.first;
        KeyCode key = pair.second;

        size_t idx = static_cast<size_t>(action);
        bool down = (key < numKeys) ? state[key] : false;

        actionPressed[idx] = down && !actionDown[idx];
        actionReleased[idx] = !down && actionDown[idx];
        actionDown[idx] = down;
    }

    bool mouseState = false;
    SDL_MouseButtonFlags mb_flags = SDL_GetMouseState(NULL, NULL);
    mouseState = (mb_flags & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)) != 0;

    mousePressed = mouseState && !mouseDown;
    mouseReleased = !mouseState && mouseDown;
    mouseDown = mouseState;
}

bool isDown(Action action){ return actionDown[static_cast<size_t>(action)]; }
bool isPressed(Action action){ return actionPressed[static_cast<size_t>(action)]; }
bool isReleased(Action action){ return actionReleased[static_cast<size_t>(action)]; }

bool isMouseDown() { return mouseDown; }
bool isMousePressed() { return mousePressed; }
bool isMouseReleased() { return mouseReleased; }

}
#include <unordered_map>
#include <array>
#include <SDL3/SDL.h>
#include <algorithm>
#include <iostream>

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

void poll() {
    resetFrame();

    SDL_PumpEvents();

    int numKeys;
    const bool* state = SDL_GetKeyboardState(&numKeys);

    for (auto& [action, key] : keyBindings) {
        size_t idx = static_cast<size_t>(action);
        bool down = (key < numKeys) ? state[key] : false;

        actionPressed[idx]  = down && !actionDown[idx];
        actionReleased[idx] = !down && actionDown[idx];
        actionDown[idx]     = down;
    }
}

bool isDown(Action action)    { return actionDown[static_cast<size_t>(action)]; }
bool isPressed(Action action) { return actionPressed[static_cast<size_t>(action)]; }
bool isReleased(Action action){ return actionReleased[static_cast<size_t>(action)]; }

}
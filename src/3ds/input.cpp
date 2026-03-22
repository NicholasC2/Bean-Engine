#include <unordered_map>
#include <array>
#include <3ds.h>
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

bool prevMouseState = false;

constexpr size_t ActionCount = static_cast<size_t>(Action::COUNT);

static std::array<bool, ActionCount> actionDown{};
static std::array<bool, ActionCount> actionPressed{};
static std::array<bool, ActionCount> actionReleased{};

void setDefaultBindings() {

}

void resetFrame() {
    std::fill(actionPressed.begin(), actionPressed.end(), false);
    std::fill(actionReleased.begin(), actionReleased.end(), false);
}

static bool mouseDown = false;
static bool mousePressed = false;
static bool mouseReleased = false;

void getMousePos(float* x, float* y) {
    if (x) *x = 0;
    if (y) *y = 0;
}

bool getMouseState() {
    return 0;
}


void poll() {
    resetFrame();

    bool mouseState = false;

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
#pragma once

#include <SDL3/SDL.h>
#include "engine/input.h"
#include "engine/assets.h"

namespace Texture {
    struct Texture;
}

namespace Renderer {

extern SDL_Window* window;
extern SDL_Renderer* renderer;

bool initRenderer(const char* title);

void shutdownRenderer();

void clearScreen();

void presentScreen();

bool showError(const char* msg);

void pollEvents(bool& running);

void drawTextureOnScreen(Texture::Texture* tex, float x, float y, float w, float h, float angle = 0.0f, int screen = 0);

bool displayLogo(Texture::Texture* tex);

}
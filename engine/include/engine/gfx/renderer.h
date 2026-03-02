#pragma once

#include "engine/gfx/texture.h"
#include "engine/input.h"
#include "engine/assets.h"
#include <SDL3/SDL.h>

namespace Renderer {

static SDL_Window* window;
static SDL_Renderer* renderer;

bool initRenderer(const char* title);

void shutdownRenderer();

void clearScreen();

void presentScreen();

bool showError(const char* msg);

void pollEvents(bool& running);

void drawTextureOnScreen(Texture::Texture* tex, float x, float y, float w, float h, float angle, int screen);

bool displayLogo(Texture* tex);

}
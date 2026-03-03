#pragma once

#include "engine/texture.h"
#include "engine/input.h"
#include "engine/assets.h"
#include <SDL3/SDL.h>

namespace Renderer {
    
SDL_Window* window;
SDL_Renderer* renderer;

bool initRenderer(const char* title);

void shutdownRenderer();

void clearScreen();

void presentScreen();

bool showError(const char* msg);

void pollEvents(bool& running);

void drawTexture2D(Texture::Texture* texture, float x, float y, float scale, int alpha, float angle, int screen);

bool displayLogo(Texture::Texture* texture, float scale);

}
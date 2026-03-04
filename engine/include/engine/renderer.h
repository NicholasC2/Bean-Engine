#pragma once

#include "engine/texture.h"
#include "engine/input.h"
#include "engine/assets.h"
#include <SDL3/SDL.h>

namespace Renderer {
    
extern SDL_Window* window;
extern SDL_Renderer* renderer;

bool initRenderer(const char* title);

void shutdownRenderer();

void clearScreen();

void presentScreen();

bool showError(const char* msg);

void pollEvents(bool& running);

void drawTexture2D(Texture::Texture* texture, float x, float y, float scale, int alpha = 255, float angle = 0.0f, int screen = 0);

bool displayLogo(Texture::Texture* texture, float scale = 1.0f);

}
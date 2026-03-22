#pragma once

#include "engine/texture.h"
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

void drawTexture2D(Texture::Texture* texture, float x, float y, float scale = 1.0f, int alpha = 255, float angle = 0.0f, int screen = 0);

bool displayLogo(Texture::Texture* texture, float scale = 1.0f);

}
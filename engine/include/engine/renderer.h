#pragma once

#include "engine/input.h"
#include "engine/assets.h"
#include <SDL3/SDL.h>

namespace Renderer {

bool initRenderer(const char* title);

void shutdownRenderer();

void clearScreen();

void presentScreen();

bool showError(const char* msg);

void pollEvents(bool& running);

void drawTexture2D(Assets::Asset* asset, float x, float y, float scale, int alpha, float angle = 0.0f, int screen = 0);

bool displayLogo(Assets::Asset* asset, float scale = 0.3f);

}
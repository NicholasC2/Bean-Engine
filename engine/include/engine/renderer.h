#pragma once

#include "engine/texture.h"
#include <SDL3/SDL.h>

namespace Texture {
    class Texture;
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

void drawTexture2D(Texture::Texture* texture, float x, float y, float scale = 1.0f, int alpha = 255, float angle = 0.0f, int screen = 0);
void drawTexture2D(Texture::Texture* texture, float x, float y, float scale = 1.0f, int alpha = 255, float angle = 0.0f, int screen = 0);

bool displayLogo(Texture::Texture* texture, float scale = 1.0f);

void drawTriangle3DTextured(
    Texture::Texture* texture,
    float x1, float y1, float z1, float u1, float v1,
    float x2, float y2, float z2, float u2, float v2,
    float x3, float y3, float z3, float u3, float v3,
    float fov);
bool displayLogo(Texture::Texture* texture, float scale = 1.0f);

}
#include "engine/gfx/window.h"
#include "engine/gfx/texture.h"
#include "engine/input.h"
#include "engine/assets.h"
#include <SDL3/SDL.h>

namespace Renderer {

static SDL_Renderer* renderer;

bool initRenderer();

void shutdownRenderer();

void clearScreen();

void presentScreen();

void drawTextureOnScreen(Texture::Texture* tex, float x, float y, float w, float h, float angle, int screen);

bool displayLogo(Texture* tex);

}
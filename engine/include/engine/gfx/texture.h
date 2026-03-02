#pragma once

#include "engine/gfx/renderer.h"
#include "engine/logo_bmp.h"
#include "engine/assets.h"
#include <SDL3/SDL.h>

namespace Texture {

struct Texture {
    SDL_Texture* handle;
    int w;
    int h;
};

void setTextureAlpha(Texture* tex, int alpha);

Texture* loadTexture(const Assets::Asset* asset);

void freeTexture(Texture*& tex);

}
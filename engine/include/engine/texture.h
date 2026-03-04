#pragma once

#include "engine/assets.h"
#include <SDL3/SDL.h>

namespace Texture {

    struct Texture {
        SDL_Texture* handle;
    };

    Texture* loadTexture(Assets::Asset* asset, int alpha = 255);

    void freeTexture(Texture* tex);
}
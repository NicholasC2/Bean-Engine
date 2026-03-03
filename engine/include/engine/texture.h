#pragma once

#include "engine/assets.h"
#include "engine/renderer.h"
#include <SDL3/SDL.h>

namespace Texture {
    struct Texture {
        SDL_Texture* handle;
    };

    Texture* loadTexture(Assets::Asset* asset, int alpha);

    void freeTexture(Texture* tex);
}
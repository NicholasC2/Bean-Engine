#pragma once

#include <SDL3/SDL.h>

namespace Assets {
    class Asset;
}

namespace Texture {
    struct Texture {
        SDL_Texture* handle;
    };

    Texture* loadTexture(Assets::Asset* asset, int alpha = 255);

    void freeTexture(Texture* tex);
}
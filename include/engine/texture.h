#pragma once

#include "engine/assets.h"

namespace Texture {

    struct Texture {
        GLuint handle;
        int width;
        int height;
    };

    Texture* loadTexture(Assets::Asset* asset, int alpha = 255);

    void freeTexture(Texture* tex);
}
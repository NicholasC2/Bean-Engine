#pragma once

#include <SDL3/SDL.h>
#include <glad/glad.h>

namespace Assets {
    class Asset;
}

namespace Texture {

    struct Texture {
        GLuint handle;
        int width;
        int height;
    };

    Texture* loadTexture(Assets::Asset* asset, int alpha = 255);

    void freeTexture(Texture* tex);
}
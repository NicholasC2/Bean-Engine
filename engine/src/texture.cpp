#include "engine/texture.h"
#include "engine/assets.h"
#include "engine/renderer.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

namespace Texture {

inline bool ends_with(const std::string& value, const std::string& ending) {
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

Texture* loadTexture(Assets::Asset* asset, int alpha) {
    if (!asset) return nullptr;

    SDL_IOStream* io = SDL_IOFromMem((void*)asset->getData(), asset->getSize());
    if (!io) {
        Renderer::showError(SDL_GetError());
        return nullptr;
    }

    SDL_Surface* surface = nullptr;

    std::string path = asset->getPath();

    if (ends_with(path, ".png")) {
        surface = SDL_LoadPNG_IO(io, 1);
    } 
    else if (ends_with(path, ".bmp")) {
        surface = SDL_LoadBMP_IO(io, 1);
    }

    if (!surface) {
        Renderer::showError(SDL_GetError());
        return nullptr;
    }

    SDL_Surface* formatted = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(surface);

    if (!formatted) {
        Renderer::showError(SDL_GetError());
        return nullptr;
    }

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        formatted->w,
        formatted->h,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        formatted->pixels
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    SDL_DestroySurface(formatted);

    Texture* tex = new Texture;
    tex->handle = texID;
    tex->width = formatted->w;
    tex->height = formatted->h;

    return tex;
}

void freeTexture(Texture* tex) {
    if (!tex) return;

    if (tex->handle) {
        glDeleteTextures(1, &tex->handle);
        tex->handle = 0;
    }

    delete tex;
}

}
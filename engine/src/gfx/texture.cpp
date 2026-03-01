#include "engine/gfx/texture.h"
#include "engine/gfx/renderer.h"
#include "engine/logo_bmp.h"
#include "engine/assets.h"
#include <SDL3/SDL.h>

namespace Texture {

struct Texture {
    SDL_Texture* handle;
};

Texture* getDefaultLogo() {
    SDL_IOStream* io = SDL_IOFromMem((void*)__logo_bmp, __logo_bmp_size);
    SDL_Surface* surface = SDL_LoadBMP_IO(io, 1);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer::renderer, surface);
    SDL_DestroySurface(surface);

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    Texture* tex = new Texture{ texture };

    return tex;
}

void setTextureAlpha(Texture* tex, int alpha) {
    if(!tex) return;
    SDL_SetTextureAlphaMod(tex->handle, alpha);
}

inline bool ends_with(std::string const & value, std::string const & ending) {
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

Texture* loadTexture(const char* path) {
    Assets::Asset *asset = Assets::getAsset(path);
    if (!asset || !asset->isValid())
        return nullptr;

    SDL_IOStream* io = SDL_IOFromMem((void*)asset->getData(), asset->getSize());
    if (!io) {
        Window::showError(SDL_GetError());
        return nullptr;
    }

    SDL_Surface* surface = nullptr;
    
    if(ends_with(std::string(path), ".png")) {
        surface = SDL_LoadPNG_IO(io, 1);
    } else if(ends_with(std::string(path), ".bmp")) {
        surface = SDL_LoadBMP_IO(io, 1);
    }

    if (!surface) {
        Window::showError(SDL_GetError());
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer::renderer, surface);
    SDL_DestroySurface(surface);

    if (!texture) {
        Window::showError(SDL_GetError());
        return nullptr;
    }

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    Texture* wrapper = new Texture{texture};
    return wrapper;
}

void freeTexture(Texture*& tex) {
    if (!tex) return;

    if (tex->handle) {
        SDL_DestroyTexture(tex->handle);
        tex->handle = nullptr;
    }

    delete tex;
    tex = nullptr;
}

}
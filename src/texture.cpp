#include "engine/texture.h"
#include "engine/assets.h"
#include "engine/renderer.h"
#include <SDL3/SDL.h>

namespace Texture {
    inline bool ends_with(std::string const & value, std::string const & ending) {
        if (ending.size() > value.size()) return false;
        return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
    }

    Texture* loadTexture(Assets::Asset* asset, int alpha) {
        if(!asset) return nullptr;

        SDL_IOStream* io = SDL_IOFromMem((void*)asset->getData(), asset->getSize());
        if (!io) {
            Renderer::showError(SDL_GetError());
            return nullptr;
        }

        SDL_Surface* surface = nullptr;

        std::string path = asset->getPath();
        
        if(ends_with(path, ".png")) {
            surface = SDL_LoadPNG_IO(io, 1);
        } else if(ends_with(std::string(path), ".bmp")) {
            surface = SDL_LoadBMP_IO(io, 1);
        }

        if (!surface) {
            Renderer::showError(SDL_GetError());
            return nullptr;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer::renderer, surface);
        SDL_DestroySurface(surface);

        if (!texture) {
            Renderer::showError(SDL_GetError());
            return nullptr;
        }

        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(texture, alpha);

        return new Texture{texture};
    }

    void freeTexture(Texture* tex) {
        if (!tex) return;

        if (tex->handle) {
            SDL_DestroyTexture(tex->handle);
            tex->handle = nullptr;
        }

        delete tex;
    }
}
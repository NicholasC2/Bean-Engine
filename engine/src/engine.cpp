#include "engine/gfx/renderer.h"
#include "engine/logo_bmp.h"

namespace Engine {

    void init() {
        if (!Renderer::initRenderer()) {
            Render::showError("failed to initialize SDL Renderer");
            return;
        }

        if(!Assets::loadAllAssets()) {
            Renderer::showError("failed to load assets!");
            return;
        }
        
        Input::setDefaultBindings();
        
        if (!Renderer::displayLogo(Renderer::getDefaultLogo())) {
            shutdown();
            return;
        }

        return;
    }

    void shutdown() {
        Render::shutdownRenderer();
        Assets::unloadAll();
        return;
    }

    Texture* getDefaultLogo() {
        Assets::Asset{}

        SDL_IOStream* io = SDL_IOFromMem((void*)__logo_bmp, __logo_bmp_size);
        SDL_Surface* surface = SDL_LoadBMP_IO(io, 1);

        SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer::renderer, surface);
        SDL_DestroySurface(surface);

        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

        Texture* tex = new Texture{ texture };

        return tex;
    }

}
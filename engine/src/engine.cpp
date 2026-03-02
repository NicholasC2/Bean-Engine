#include "engine/gfx/renderer.h"
#include "engine/gfx/texture.h"
#include "engine/logo_bmp.h"
#include "engine/engine.h"

namespace Engine {

    bool init(const char* title) {
        if (!Renderer::initRenderer(title)) {
            Renderer::showError("failed to initialize SDL Renderer");
            return false;
        }

        if(!Assets::loadAllAssets()) {
            Renderer::showError("failed to load assets!");
            return false;
        }
        
        Input::setDefaultBindings();

        Assets::Asset default_logo(
            "default_logo.bmp",
            std::vector<uint8_t>(__logo_bmp, __logo_bmp + __logo_bmp_size)
        );

        Texture::Texture* logo_tex = Texture::loadTexture(&default_logo);
        
        if (!Renderer::displayLogo(logo_tex)) {
            Texture::freeTexture(logo_tex);
            return false;
        }

        return true;
    }

    void shutdown() {
        Renderer::shutdownRenderer();
        Assets::unloadAll();
        return;
    }

}
#include "engine/platform.h"
#include "engine/logo_bmp.h"

namespace Engine {

    bool displayLogo(Texture::Texture* texture,float scale) {
        Input::setDefaultBindings();

        float elapsed = 0.0f;
        bool running = true;

        while(elapsed < 3 && running)
        {
            Renderer::pollEvents(running);
            Renderer::clearScreen();

            Uint8 alpha = 255;

            if(elapsed < 1)
                alpha=(Uint8)((elapsed)*255.0f);
            else if(elapsed > 2 && elapsed < 3)
            {
                alpha=(Uint8)((1.0f-(elapsed-2))*255.0f);
            }

            if(elapsed < 3)
                Renderer::drawTexture2D(texture,0.9f,0.5f,scale,alpha);

            Input::poll();

            if(Input::isPressed(Input::Action::Jump) || Input::isMousePressed())
                return true;

            Renderer::presentScreen();

            elapsed += Renderer::getDeltaTime();
        }

        return running;
    }

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

        Texture::Texture* default_logo_tex = Texture::loadTexture(&default_logo);
        
        if (!Engine::displayLogo(default_logo_tex, 0.3f)) {
            Texture::freeTexture(default_logo_tex);
            return false;
        }

        Texture::freeTexture(default_logo_tex);

        return true;
    }

    void shutdown() {
        Renderer::shutdownRenderer();
        Assets::unloadAll();
        return;
    }

}
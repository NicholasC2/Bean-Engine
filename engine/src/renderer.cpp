#include "engine/renderer.h"
#include "engine/input.h"
#include "engine/assets.h"
#include <SDL3/SDL.h>

namespace Renderer {
    
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

bool initRenderer(const char* title) {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR ,"SDL_Init failed", SDL_GetError(), NULL);
        return false;
    }

    window = SDL_CreateWindow(title, 900, 500, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!window) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR ,"SDL_CreateWindow failed", SDL_GetError(), NULL);
        return false;
    }

    SDL_SetWindowAspectRatio(window, 9.0f / 5.0f, 9.0f / 5.0f);

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR ,"SDL_CreateRenderer failed", SDL_GetError(), NULL);
        return false;
    }

    SDL_SetDefaultTextureScaleMode(renderer, SDL_SCALEMODE_PIXELART);

    return true;
}

void shutdownRenderer() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
}

void clearScreen() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void presentScreen() {
    SDL_RenderPresent(renderer);
}

bool showError(const char* msg) {
    return SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", msg, window);
}

void pollEvents(bool& running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            running = false;
        }
    }
}

inline bool ends_with(std::string const & value, std::string const & ending) {
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

SDL_Texture* getTexture(Assets::Asset* asset, int alpha) {
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

    return texture;
}

void drawTexture2D(Assets::Asset* asset, float x, float y, float scale, int alpha, float angle, int screen) {
    if (!renderer || !asset || !asset->isValid() || !window) return;

    int winW, winH;
    SDL_GetWindowSizeInPixels(window, &winW, &winH);

    float px = x * winH;
    float py = y * winH;

    SDL_Texture *texture = getTexture(asset, alpha);

    float pw = (float(winH) * scale) * (float(texture->w) / float(texture->h));
    float ph = float(winH) * scale;
    
    SDL_FRect dstRect = { px - (pw / 2.0f), py - (ph / 2.0f), pw, ph };
    SDL_FPoint center = { pw / 2.0f, ph / 2.0f };

    SDL_RenderTextureRotated(renderer, texture, nullptr, &dstRect, angle, &center, SDL_FLIP_NONE);
}

bool displayLogo(Assets::Asset* asset, float scale) {
    if(!asset) return false;

    Input::setDefaultBindings();
    Uint64 startTicks = SDL_GetTicks();
    bool running = true;
    int i = 0;

    while(SDL_GetTicks() - startTicks < 4000 && running) {
        Uint64 elapsed = SDL_GetTicks() - startTicks;

        pollEvents(running);
        clearScreen();

        Uint8 alpha = 255;

        if (elapsed < 1000) {
            alpha = (Uint8)((elapsed / 1000.0f) * 255.0f);
        }
        else if (elapsed > 2000 && elapsed < 3000) {
            float fadeProgress = (elapsed - 2000) / 1000.0f;
            alpha = (Uint8)((1.0f - fadeProgress) * 255.0f);
        }
        
        if(elapsed < 3000) { 
            drawTexture2D(asset, 0.9f, 0.5f, scale, alpha);
        }

        Input::poll();

        if (Input::isPressed(Input::Action::Jump) || Input::isMousePressed()) {
            return true;
        }

        presentScreen();

        i++;
    }

    return running;
}

}
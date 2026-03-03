#include "engine/renderer.h"
#include "engine/texture.h"
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

void drawTexture2D(Texture::Texture* texture, float x, float y, float scale, int alpha, float angle, int screen) {
    if (!renderer || !texture || !texture->handle || !window) return;

    int winW, winH;
    SDL_GetWindowSizeInPixels(window, &winW, &winH);

    SDL_SetTextureBlendMode(texture->handle, SDL_BLENDMODE_BLEND);

    SDL_SetTextureAlphaMod(texture->handle, alpha);

    float px = x * winH;
    float py = y * winH;

    float pw = (float(winH) * scale) * (float(texture->handle->w) / float(texture->handle->h));
    float ph = float(winH) * scale;
    
    SDL_FRect dstRect = { px - (pw / 2.0f), py - (ph / 2.0f), pw, ph };
    SDL_FPoint center = { pw / 2.0f, ph / 2.0f };

    SDL_RenderTextureRotated(renderer, texture->handle, nullptr, &dstRect, angle, &center, SDL_FLIP_NONE);
}

bool displayLogo(Texture::Texture* texture, float scale) {
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
            drawTexture2D(texture, 0.9f, 0.5f, scale, alpha);
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
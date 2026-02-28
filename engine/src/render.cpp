#include "render.h"
#include "logo_bmp.h"
#include <SDL3/SDL.h>
#include <chrono>
#include <string>
#include "input.h"
#include "assets.h"

namespace Render {

struct Texture {
    SDL_Texture* handle;
};

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

bool initWindow(int width, int height, const char* title) {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR ,"SDL_Init failed", SDL_GetError(), NULL);
        return false;
    }

    window = SDL_CreateWindow(title, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!window) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR ,"SDL_CreateWindow failed", SDL_GetError(), NULL);
        return false;
    }

    SDL_SetWindowAspectRatio(window, 16.2 / 9, 16.2 / 9);

    return true;
}

void getWindowSize(int &w, int &h) {
    if (!window) {
        w = 0;
        h = 0;
        return;
    }

    SDL_GetWindowSizeInPixels(window, &w, &h);
}

Texture* getDefaultLogo() {
    SDL_IOStream* io = SDL_IOFromMem((void*)__logo_bmp, __logo_bmp_size);
    SDL_Surface* surface = SDL_LoadBMP_IO(io, 1);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    Texture* tex = new Texture{ texture };

    return tex;
}

bool showError(const char* msg) {
    return SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", msg, window);
}

bool displayLogo(Texture* tex) {
    SDL_SetTextureBlendMode(tex->handle, SDL_BLENDMODE_BLEND);

    Input::setDefaultBindings();
    Uint64 startTicks = SDL_GetTicks();
    bool running = true;
    int i = 0;

    float ratio = (float)tex->handle->h / (float)tex->handle->w;

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
            setTextureAlpha(tex, alpha);

            drawTextureOnScreen(tex, 0.9f, 0.5f, 0.7f, ratio * 0.7f, 0.0f);
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

void setTextureAlpha(Texture* tex, int alpha) {
    SDL_SetTextureAlphaMod(tex->handle, alpha);
}

bool initRenderer() {
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
}

void shutdownWindow() {
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}

void pollEvents(bool& running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            running = false;
        }
    }
}

void clearScreen() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void presentScreen() {
    SDL_RenderPresent(renderer);
}

inline bool ends_with(std::string const & value, std::string const & ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

Texture* loadTexture(const char* path) {
    Assets::Asset *asset = Assets::getAsset(path);
    if (!asset || !asset->isValid())
        return nullptr;

    SDL_IOStream* io = SDL_IOFromMem((void*)asset->getData(), asset->getSize());
    if (!io) {
        showError(SDL_GetError());
        return nullptr;
    }

    SDL_Surface* surface = nullptr;
    
    if(ends_with(std::string(path), ".png")) {
        surface = SDL_LoadPNG_IO(io, 1);
    } else if(ends_with(std::string(path), ".bmp")) {
        surface = SDL_LoadBMP_IO(io, 1);
    }

    if (!surface) {
        showError(SDL_GetError());
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);

    if (!texture) {
        showError(SDL_GetError());
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

void drawTextureOnScreen(Texture* tex, float x, float y, float w, float h, float angle, int screen)
{
    if (!renderer || !tex || !tex->handle || !window) return;

    int winW, winH;
    SDL_GetWindowSizeInPixels(window, &winW, &winH);

    float px = x * winH;
    float py = y * winH;
    float pw = w * winH;
    float ph = h * winH;

    SDL_FRect dstRect = { px - (pw / 2.0f), py - (ph / 2.0f), pw, ph };
    SDL_FPoint center = { pw / 2.0f, ph / 2.0f };

    SDL_RenderTextureRotated(renderer, tex->handle, nullptr, &dstRect, angle, &center, SDL_FLIP_NONE);
}

}
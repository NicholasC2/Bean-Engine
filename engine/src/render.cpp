#include "render.h"
#include "logo_bmp.h"
#include <SDL3/SDL.h>
#include <chrono>

namespace Render {

struct Texture {
    SDL_Texture* handle;
};

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

bool initWindow(int width, int height, const char* title) {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return false;
    }

    window = SDL_CreateWindow(title, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
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

bool initRenderer() {
    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
        return false;
    }

    SDL_IOStream* io = SDL_IOFromMem((void*)logo_bmp, logo_bmp_size);
    SDL_Surface* surface = SDL_LoadBMP_IO(io, 1);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    Texture* tex = new Texture{texture};

    Uint32 frameStart;
    int frameTime;
    bool running = true;
    int opacity = 0;
    int i = 0;
    while(i < 120 && running) {
        frameStart = SDL_GetTicks();

        pollEvents(running);
        clearScreen();

        if(i < 30) {
            opacity++;
        }
        if(i >= 90) {
            opacity--;
        }
        drawTexture(tex, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f);
        
        presentScreen();

        frameTime = SDL_GetTicks() - frameStart;
        if (1000 / 60 > frameTime) {
            SDL_Delay(1000 / 60 - frameTime);
        }

        i++;
    }

    SDL_DestroySurface(surface);
    return running;
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

Texture* loadTexture(const char* path) {
    SDL_Surface* surface = SDL_LoadBMP(path);
    if (!surface) return nullptr;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    if (!tex) return nullptr;
    Texture* wrapper = new Texture{tex};
    return wrapper;
}

void freeTexture(Texture* tex) {
    if (!tex) return;
    if (tex->handle) SDL_DestroyTexture(tex->handle);
    delete tex;
}

void drawTexture(Texture* tex, float x, float y, float w, float h, float angle) {
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
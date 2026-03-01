#include "engine/gfx/window.h"
#include <SDL3/SDL.h>

namespace Window {

struct Window {
    SDL_Window* handle;
};

static SDL_Window* window = nullptr;

bool initWindow(const char* title) {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR ,"SDL_Init failed", SDL_GetError(), NULL);
        return false;
    }

    window = SDL_CreateWindow(title, 900, 500, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!window) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR ,"SDL_CreateWindow failed", SDL_GetError(), NULL);
        return false;
    }

    SDL_SetWindowAspectRatio(window, 9 / 5, 9 / 5);

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

bool showError(const char* msg) {
    return SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", msg, window);
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

}
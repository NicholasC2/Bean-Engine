#include "engine/renderer.h"
#include "engine/texture.h"
#include "engine/input.h"
#include "engine/assets.h"

#include <cmath>
#include <SDL3/SDL.h>

namespace Renderer {

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

bool initRenderer(const char* title) {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        Renderer::showError(SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow(title, 900, 500, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!window) {
        Renderer::showError(SDL_GetError());
        return false;
    }

    SDL_SetWindowAspectRatio(window, 9.0f / 5.0f, 9.0f / 5.0f);

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        Renderer::showError(SDL_GetError());
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

static void projectPoint(float x, float y, float z, float fov, int screenW, int screenH, float& outX, float& outY) {
    float aspect = float(screenW) / float(screenH);
    float fovRad = 1.0f / tanf((fov * 0.5f) * (3.14159265f / 180.0f));

    if (z <= 0.01f) z = 0.01f;

    float px = (x * fovRad / z) / aspect;
    float py = (y * fovRad / z);

    outX = (px + 1.0f) * 0.5f * screenW;
    outY = (1.0f - py) * 0.5f * screenH;
}

static float edgeFunction(float ax, float ay, float bx, float by, float cx, float cy)
{
    return (cx - ax) * (by - ay) - (cy - ay) * (bx - ax);
}

void drawTriangle3DTextured(
    Texture::Texture* texture,
    float x1, float y1, float z1, float u1, float v1,
    float x2, float y2, float z2, float u2, float v2,
    float x3, float y3, float z3, float u3, float v3,
    float fov)
{
    if (!renderer || !texture || !texture->handle || !window) return;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    int w, h;
    SDL_GetWindowSizeInPixels(window, &w, &h);

    float sx1, sy1, sx2, sy2, sx3, sy3;
    projectPoint(x1, y1, z1, fov, w, h, sx1, sy1);
    projectPoint(x2, y2, z2, fov, w, h, sx2, sy2);
    projectPoint(x3, y3, z3, fov, w, h, sx3, sy3);

    SDL_Vertex verts[3];

    verts[0].position = { sx1, sy1 };
    verts[0].color = { 1, 1, 1, 1 };
    verts[0].tex_coord = { u1, 1.0f - v1 };

    verts[1].position = { sx2, sy2 };
    verts[1].color = { 1, 1, 1, 1 };
    verts[1].tex_coord = { u2, 1.0f - v2 };

    verts[2].position = { sx3, sy3 };
    verts[2].color = { 1, 1, 1, 1 };
    verts[2].tex_coord = { u3, 1.0f - v3 };

    SDL_RenderGeometry(renderer, texture->handle, verts, 3, nullptr, 0);
}

}
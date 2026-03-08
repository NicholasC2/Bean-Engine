#include "engine/renderer.h"
#include "engine/texture.h"
#include "engine/input.h"
#include "engine/assets.h"

#include <cmath>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

namespace Renderer {

SDL_Window* window = nullptr;
SDL_GLContext glContext = nullptr;

bool initRenderer(const char* title) {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        Renderer::showError(SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow(
        title,
        900,
        500,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        Renderer::showError(SDL_GetError());
        return false;
    }

    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        Renderer::showError(SDL_GetError());
        return false;
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    SDL_GL_SetSwapInterval(1);

    SDL_SetWindowAspectRatio(window, 9.0f / 5.0f, 9.0f / 5.0f);

    glViewport(0, 0, 1944, 1080);

    return true;
}

void shutdownRenderer() {
    if (glContext) {
        SDL_GL_DestroyContext(glContext);
        glContext = nullptr;
    }

    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
}

void clearScreen() {
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void presentScreen() {
    SDL_GL_SwapWindow(window);
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
    if (!texture) return;

    int winW, winH;
    SDL_GetWindowSizeInPixels(window, &winW, &winH);

    float px = x * winH;
    float py = y * winH;

    float pw = (winH * scale) * (texture->width / (float)texture->height);
    float ph = winH * scale;

    glBindTexture(GL_TEXTURE_2D, texture->handle);

    glPushMatrix();

    glTranslatef(px, py, 0);
    glRotatef(angle, 0, 0, 1);
    glScalef(pw, ph, 1);

    glColor4f(1,1,1, alpha / 255.0f);

    glBegin(GL_QUADS);

    glTexCoord2f(0,0); glVertex2f(-0.5f, -0.5f);
    glTexCoord2f(1,0); glVertex2f( 0.5f, -0.5f);
    glTexCoord2f(1,1); glVertex2f( 0.5f,  0.5f);
    glTexCoord2f(0,1); glVertex2f(-0.5f,  0.5f);

    glEnd();

    glPopMatrix();
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

void drawTriangle3DTextured(
    Texture::Texture* texture,
    float x1, float y1, float z1, float u1, float v1,
    float x2, float y2, float z2, float u2, float v2,
    float x3, float y3, float z3, float u3, float v3,
    float fov)
{
    if (!glContext || !texture || !texture->handle || !window) return;

    glBindTexture(GL_TEXTURE_2D, texture->handle);

    glBegin(GL_TRIANGLES);

    glTexCoord2f(u1,v1);
    glVertex3f(x1,y1,z1);

    glTexCoord2f(u2,v2);
    glVertex3f(x2,y2,z2);

    glTexCoord2f(u3,v3);
    glVertex3f(x3,y3,z3);

    glEnd();
}

}
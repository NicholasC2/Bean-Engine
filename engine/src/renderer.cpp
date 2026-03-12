#include "engine/renderer.h"
#include "engine/texture.h"
#include "engine/input.h"
#include "engine/shaders.h"

#include <glad/glad.h>
#include <cmath>

namespace Renderer {

SDL_Window* window = nullptr;
SDL_GLContext glContext = nullptr;

static GLuint prog2D = 0;
static GLuint prog3D = 0;

static GLuint quadVAO = 0, quadVBO = 0;
static GLuint triVAO = 0, triVBO = 0;

static float deltaTime = 0.0f;
static Uint64 lastTime = 0;
static int ticks = 0;

float getDeltaTime() {
    return deltaTime;
}

int getTicks() {
    return ticks;
}

static GLuint compileShader(GLenum type, const char* src) {
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);
    return s;
}

static GLuint createShaderProgram(const char* vertSrc, const char* fragSrc) {
    GLuint v = compileShader(GL_VERTEX_SHADER, vertSrc);
    GLuint f = compileShader(GL_FRAGMENT_SHADER, fragSrc);
    GLuint p = glCreateProgram();
    glAttachShader(p, v);
    glAttachShader(p, f);
    glLinkProgram(p);
    glDeleteShader(v);
    glDeleteShader(f);
    return p;
}

bool initRenderer(const char* title) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
        return showError(SDL_GetError());

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow(title, 900, 500, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if(!window) return showError(SDL_GetError());

    glContext = SDL_GL_CreateContext(window);
    glContext = SDL_GL_CreateContext(window);
    if(!glContext) return showError(SDL_GetError());

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        return showError("Failed to initialize GLAD!");
    }

    SDL_GL_SetSwapInterval(1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    prog2D = createShaderProgram(vertexShader2D, fragmentShader);
    prog3D = createShaderProgram(vertexShader3D, fragmentShader);

    float quad[] = {
        -0.5f,-0.5f,0,0,  0.5f,-0.5f,1,0,  0.5f, 0.5f,1,1,
        -0.5f,-0.5f,0,0,  0.5f, 0.5f,1,1, -0.5f, 0.5f,0,1
    };
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &triVAO);
    glGenBuffers(1, &triVBO);
    glBindVertexArray(triVAO);
    glBindBuffer(GL_ARRAY_BUFFER, triVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 15, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return true;
}

void shutdownRenderer() {
    if(prog2D) glDeleteProgram(prog2D);
    if(prog3D) glDeleteProgram(prog3D);

    if(quadVBO) glDeleteBuffers(1, &quadVBO);
    if(quadVAO) glDeleteVertexArrays(1, &quadVAO);

    if(triVBO) glDeleteBuffers(1, &triVBO);
    if(triVAO) glDeleteVertexArrays(1, &triVAO);

    if(glContext) {
        SDL_GL_DestroyContext(glContext);
        glContext = nullptr;
    }

    if(window) {
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
    return SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error!",msg,window);
}

void pollEvents(bool& running) {
    Uint64 currentTime = SDL_GetPerformanceCounter();
    if (lastTime == 0) lastTime = currentTime;

    deltaTime = (float)(currentTime - lastTime) / (float)SDL_GetPerformanceFrequency();
    lastTime = currentTime;

    ticks += 1;

    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        if(event.type==SDL_EVENT_QUIT)
            running=false;
    }
}

void drawTexture2D(Texture::Texture* texture, float x, float y, float scale, int alpha, float angle, int screen) {
    if(!texture) return;
    int winW, winH;
    SDL_GetWindowSizeInPixels(window, &winW, &winH);

    glUseProgram(prog2D);
    glUniform2f(glGetUniformLocation(prog2D, "pos"), x * winH, y * winH);
    glUniform2f(glGetUniformLocation(prog2D, "size"), winH * scale * (texture->width/(float)texture->height), winH * scale);
    glUniform2f(glGetUniformLocation(prog2D, "screen"), (float)winW, (float)winH);
    glUniform1f(glGetUniformLocation(prog2D, "angle"), angle * 0.0174533f);
    glUniform1f(glGetUniformLocation(prog2D, "alpha"), alpha / 255.0f);

    glBindTexture(GL_TEXTURE_2D, texture->handle);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawTriangle3DTextured(Texture::Texture* texture, 
                            float x1, float y1, float z1, float u1, float v1,
                            float x2, float y2, float z2, float u2, float v2,
                            float x3, float y3, float z3, float u3, float v3,
                            float fov) 
{
    if(!texture) return;

    float verts[] = {
        x1, y1, z1, u1, v1,
        x2, y2, z2, u2, v2,
        x3, y3, z3, u3, v3
    };

    int winW, winH;
    SDL_GetWindowSize(window, &winW, &winH);

    glUseProgram(prog3D);
    glUniform1f(glGetUniformLocation(prog3D, "fov"), fov);
    glUniform1f(glGetUniformLocation(prog3D, "aspect"), (float)winW / (float)winH);
    glUniform1f(glGetUniformLocation(prog3D, "alpha"), 1.0f);

    glBindTexture(GL_TEXTURE_2D, texture->handle);
    glBindVertexArray(triVAO);
    glBindBuffer(GL_ARRAY_BUFFER, triVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

}
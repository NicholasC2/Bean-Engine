#include "engine/renderer.h"
#include "engine/texture.h"
#include "engine/input.h"

#include <glad/glad.h>
#include <cmath>

namespace Renderer {

SDL_Window* window = nullptr;
SDL_GLContext glContext = nullptr;

static GLuint shaderProgram = 0;
static GLuint quadVAO = 0;
static GLuint quadVBO = 0;

static GLuint compileShader(GLenum type,const char* src)
{
    GLuint s = glCreateShader(type);
    glShaderSource(s,1,&src,nullptr);
    glCompileShader(s);
    return s;
}

static GLuint createShader()
{
    const char* vs =
        "#version 330 core\n"
        "layout(location=0) in vec2 aPos;"
        "layout(location=1) in vec2 aUV;"
        "out vec2 UV;"
        "uniform vec2 pos;"
        "uniform vec2 size;"
        "uniform vec2 screen;"
        "uniform float angle;"
        "void main(){"
        "float s=sin(angle);"
        "float c=cos(angle);"
        "vec2 p=vec2(aPos.x*c-aPos.y*s,aPos.x*s+aPos.y*c);"
        "p=p*size+pos;"
        "vec2 ndc=(p/screen)*2.0-1.0;"
        "ndc.y=-ndc.y;"
        "gl_Position=vec4(ndc,0,1);"
        "UV=aUV;"
        "}";

    const char* fs =
        "#version 330 core\n"
        "in vec2 UV;"
        "out vec4 FragColor;"
        "uniform sampler2D tex;"
        "uniform float alpha;"
        "void main(){"
        "vec4 c=texture(tex,UV);"
        "FragColor=vec4(c.rgb,c.a*alpha);"
        "}";

    GLuint v = compileShader(GL_VERTEX_SHADER,vs);
    GLuint f = compileShader(GL_FRAGMENT_SHADER,fs);

    GLuint p = glCreateProgram();
    glAttachShader(p,v);
    glAttachShader(p,f);
    glLinkProgram(p);

    glDeleteShader(v);
    glDeleteShader(f);

    return p;
}

bool initRenderer(const char* title)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
        return showError(SDL_GetError());

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow(
        title,
        900,
        500,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if(!window) return showError(SDL_GetError());

    glContext = SDL_GL_CreateContext(window);
    if(!glContext) return showError(SDL_GetError());

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        return showError("Failed to initialize GLAD!");
    }

    SDL_GL_SetSwapInterval(1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    shaderProgram = createShader();

    float quad[]={
        -0.5f,-0.5f,0,0,
         0.5f,-0.5f,1,0,
         0.5f, 0.5f,1,1,
        -0.5f,-0.5f,0,0,
         0.5f, 0.5f,1,1,
        -0.5f, 0.5f,0,1
    };

    glGenVertexArrays(1,&quadVAO);
    glGenBuffers(1,&quadVBO);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER,quadVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(quad),quad,GL_STATIC_DRAW);

    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);

    return true;
}

void shutdownRenderer()
{
    if(quadVBO) glDeleteBuffers(1,&quadVBO);
    if(quadVAO) glDeleteVertexArrays(1,&quadVAO);
    if(shaderProgram) glDeleteProgram(shaderProgram);

    if(glContext)
    {
        SDL_GL_DestroyContext(glContext);
        glContext=nullptr;
    }

    if(window)
    {
        SDL_DestroyWindow(window);
        window=nullptr;
    }

    SDL_Quit();
}

void clearScreen()
{
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void presentScreen()
{
    SDL_GL_SwapWindow(window);
}

bool showError(const char* msg)
{
    return SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error!",msg,window);
}

void pollEvents(bool& running)
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        if(event.type==SDL_EVENT_QUIT)
            running=false;
    }
}

void drawTexture2D(Texture::Texture* texture,
                   float x,float y,
                   float scale,
                   int alpha,
                   float angle,
                   int screen)
{
    if(!texture) return;

    int winW,winH;
    SDL_GetWindowSizeInPixels(window,&winW,&winH);

    float px=x*winH;
    float py=y*winH;

    float ph=winH*scale;
    float pw=ph*(texture->width/(float)texture->height);

    glUseProgram(shaderProgram);

    glUniform2f(glGetUniformLocation(shaderProgram,"pos"),px,py);
    glUniform2f(glGetUniformLocation(shaderProgram,"size"),pw,ph);
    glUniform2f(glGetUniformLocation(shaderProgram,"screen"),(float)winW,(float)winH);
    glUniform1f(glGetUniformLocation(shaderProgram,"angle"),angle*0.0174533f);
    glUniform1f(glGetUniformLocation(shaderProgram,"alpha"),alpha/255.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texture->handle);

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES,0,6);
}

bool displayLogo(Texture::Texture* texture,float scale)
{
    Input::setDefaultBindings();

    Uint64 startTicks=SDL_GetTicks();
    bool running=true;

    while(SDL_GetTicks()-startTicks<4000 && running)
    {
        Uint64 elapsed=SDL_GetTicks()-startTicks;

        pollEvents(running);
        clearScreen();

        Uint8 alpha=255;

        if(elapsed<1000)
            alpha=(Uint8)((elapsed/1000.0f)*255.0f);
        else if(elapsed>2000 && elapsed<3000)
        {
            float fade=(elapsed-2000)/1000.0f;
            alpha=(Uint8)((1.0f-fade)*255.0f);
        }

        if(elapsed<3000)
            drawTexture2D(texture,0.9f,0.5f,scale,alpha);

        Input::poll();

        if(Input::isPressed(Input::Action::Jump) || Input::isMousePressed())
            return true;

        presentScreen();
    }

    return running;
}

void drawTriangle3DTextured(
    Texture::Texture* texture,
    float x1,float y1,float z1,float u1,float v1,
    float x2,float y2,float z2,float u2,float v2,
    float x3,float y3,float z3,float u3,float v3,
    float fov)
{
    if(!texture) return;

    float verts[]={
        x1,y1,u1,v1,
        x2,y2,u2,v2,
        x3,y3,u3,v3
    };

    GLuint vao,vbo;

    glGenVertexArrays(1,&vao);
    glGenBuffers(1,&vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(verts),verts,GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);

    glUseProgram(shaderProgram);

    glBindTexture(GL_TEXTURE_2D,texture->handle);

    glDrawArrays(GL_TRIANGLES,0,3);

    glDeleteBuffers(1,&vbo);
    glDeleteVertexArrays(1,&vao);
}

}
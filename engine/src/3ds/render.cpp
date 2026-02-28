#include "render.h"
#include "logo_bmp.h"
#include "input.h"
#include "assets.h"

#include <3ds.h>
#include <citro2d.h>

#include <chrono>
#include <string>
#include <cstring>
#include <algorithm>

namespace Render {

struct Texture {
    C2D_Image handle;
    C2D_SpriteSheet_s* sheet;
    int alpha;
};

static u8* topFramebuffer = nullptr;
static u16 screenWidth = 400;
static u16 screenHeight = 240;

bool initWindow(int width, int height, const char* title) {
	gfxInitDefault();
    consoleInit(GFX_TOP, nullptr);
    return true;
}

void getWindowSize(int &w, int &h) {
    w = screenWidth;
    h = screenHeight;
}

Texture* getDefaultLogo() {
    Texture* tex = new Texture;
    C2D_SpriteSheet_s* sheet = C2D_SpriteSheetLoadFromMem(__logo_bmp, __logo_bmp_size);
    if (!sheet) { delete tex; return nullptr; }

    tex->sheet = sheet;
    tex->handle = C2D_SpriteSheetGetImage(sheet, 0);
    tex->alpha = 255;

    return tex;
}


bool showError(const char* msg) {
    consoleClear();
    return false;
}

bool displayLogo(Texture* tex) {
    using namespace std::chrono;
    Input::setDefaultBindings();
    bool running = true;
    
    auto start = high_resolution_clock::now();
    float ratio = float(tex->handle.tex->height) / float(tex->handle.tex->width);
    int alpha = 0;

    while(aptMainLoop()) {
        auto now = high_resolution_clock::now();
        auto elapsed = duration_cast<milliseconds>(now - start).count();
        if (elapsed > 4000) break;

        pollEvents(running);
        clearScreen();

        if (elapsed < 1000)
            alpha = int(255.0f * elapsed / 1000.0f);
        else if (elapsed > 2000 && elapsed < 3000)
            alpha = int(255.0f * (1.0f - float(elapsed - 2000) / 1000.0f));

        if (elapsed < 3000) {
            setTextureAlpha(tex, alpha);
            drawTextureOnScreen(tex, 0.5f, 0.5f, 1.0f, ratio, 0.0f, GFX_TOP);
        }

        Input::poll();
        if (Input::isPressed(Input::Action::Jump) || Input::isMousePressed())
            return true;

        presentScreen();
    }

    return aptMainLoop();
}

void setTextureAlpha(Texture* tex, int alpha) {
    if (!tex) return;
    alpha = std::clamp(alpha, 0, 255);

    tex->alpha = alpha;
}

bool initRenderer() {
    C2D_Init(999);
    return true;
}

void shutdownRenderer() {

}

void shutdownWindow() {

}

void pollEvents(bool& running) {
    hidScanInput();
    running = aptMainLoop();
}

void clearScreen() {
    topFramebuffer = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, &screenWidth, &screenHeight);
    if (topFramebuffer) memset(topFramebuffer, 0, screenWidth * screenHeight * 3);
}

void presentScreen() {
	gfxFlushBuffers();
	gfxSwapBuffers();
	gspWaitForVBlank();
}

Texture* loadTexture(const char* path) {
    Assets::Asset* asset = Assets::getAsset(path);
    if (!asset || !asset->isValid()) return nullptr;

    Texture* tex = new Texture;
    const uint8_t* data = asset->getData();
    size_t size = asset->getSize();

    C2D_SpriteSheet_s* sheet = C2D_SpriteSheetLoadFromMem(data, size);
    if (!sheet) { delete tex; return nullptr; }

    tex->handle = C2D_SpriteSheetGetImage(sheet, 0);
    tex->alpha = 255;
    tex->sheet = sheet;

    return tex;
}

void freeTexture(Texture*& tex) {
    if (!tex) return;
    if (tex) linearFree(tex);
    delete tex;
    tex = nullptr;
}

void drawTextureOnScreen(Texture* tex, float x, float y, float w, float h, float angle, int screen) {
    if (!tex) return;

    float scaleX = w;
    float scaleY = h;

    C2D_DrawImageAtRotated(tex->handle, x * screenWidth, y * screenHeight, 0.0f, angle, nullptr, scaleX, scaleY);
}

}
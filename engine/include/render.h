#include "logo_bmp.h"
#include <chrono>
#include <string>

namespace Render {

struct Texture;

bool initWindow(int width, int height, const char* title);
void getWindowSize(int &w, int &h);
Texture* getDefaultLogo();
bool displayLogo(Texture* tex);
void setTextureAlpha(Texture* tex, int alpha);
bool initRenderer();
void shutdownRenderer();
void shutdownWindow();
void pollEvents(bool& running);
void clearScreen();
void presentScreen();
Texture* loadTexture(const char* path);
void freeTexture(Texture*& tex);
void drawTextureOnScreen(Texture* tex, float x, float y, float w, float h, float angle = 0.0f, int screen = 0);

}
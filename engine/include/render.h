#include <iostream>

namespace Render {

struct Texture;

bool initWindow(int width, int height, const char* title);
void getWindowSize(int &w, int &h);
bool initRenderer();
void shutdownRenderer();
void shutdownWindow();
void pollEvents(bool& running);
void clearScreen();
void presentScreen();
Texture* loadTexture(const char* path);
void freeTexture(Texture* tex);
void drawTexture(Texture* tex, float x, float y, float w, float h, float angle = 0.0f);

}
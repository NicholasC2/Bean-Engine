#include <SDL3/SDL.h>

namespace Window {

struct Window;

static SDL_Window* window;

bool initWindow(const char* title);

void getWindowSize(int &w, int &h);

bool showError(const char* msg);

void shutdownWindow();

void pollEvents(bool& running);

}
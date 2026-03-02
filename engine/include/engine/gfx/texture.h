#pragma once

#include "engine/gfx/renderer.h"
#include "engine/logo_bmp.h"
#include "engine/assets.h"
#include <SDL3/SDL.h>

namespace Texture {

struct Texture;

void setTextureAlpha(Texture* tex, int alpha);

inline bool ends_with(std::string const & value, std::string const & ending);

Texture* loadTexture(const char* path);

void freeTexture(Texture*& tex);

}
#pragma once

#include "platform.h"

namespace Texture {
    class Texture;
}

namespace Engine {

    bool displayLogo(Texture::Texture* texture,float scale = 0.5f);

    bool init(const char* title);

    void shutdown();

}
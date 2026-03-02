#pragma once

#include "engine.h"

#if defined(PLATFORM_3DS)
    #include "3ds/input.h"
    #include "3ds/render.h"
    #include "3ds/assets.h"
#else 
    #include "input.h"
    #include "gfx/renderer.h"
    #include "gfx/texture.h"
    #include "assets.h"
#endif
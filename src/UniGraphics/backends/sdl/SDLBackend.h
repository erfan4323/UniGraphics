#pragma once

#include "UniGraphics.h"

namespace ugfx::sdl {

    class SDLBackend : public GraphicsBackend {
       public:
        SDLBackend();
        ~SDLBackend() override;
    };

}  // namespace ugfx::sdl
#pragma once

#include "CommonTypes.h"
#include "UniGraphics.h"

namespace ugfx::sdl {

    class SDLBackend : public GraphicsBackend {
       public:
        SDLBackend();
        ~SDLBackend() override;

        BackendType GetBackendType() override { return BackendType::SDL; }
    };

}  // namespace ugfx::sdl

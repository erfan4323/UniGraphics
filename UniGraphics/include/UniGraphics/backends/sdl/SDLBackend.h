#pragma once

#include "UniGraphics.h"

namespace ugfx::sdl {

    class SDLBackend : public GraphicsBackend {
       public:
        SDLBackend();
        ~SDLBackend() override;

        BackendType GetBackendType() const override { return BackendType::SDL; }
    };

}  // namespace ugfx::sdl
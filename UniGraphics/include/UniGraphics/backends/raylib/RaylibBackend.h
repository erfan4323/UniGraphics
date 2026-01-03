#pragma once

#include <raylib.h>

#include "UniGraphics.h"

namespace ugfx::raylib {

    using RayCamera = ::Camera2D;

    class RaylibBackend : public GraphicsBackend {
       public:
        RaylibBackend();
        ~RaylibBackend() override;
    };

}  // namespace ugfx::raylib

#pragma once

#include "UniGraphics.h"

namespace ugfx::raylib {

    class RaylibBackend : public GraphicsBackend {
       public:
        RaylibBackend();
        ~RaylibBackend() override;
    };

}  // namespace ugfx::raylib
#pragma once

#include "UniGraphics.h"

namespace ugfx::raylib {

    class RaylibBackend : public GraphicsBackend {
       public:
        RaylibBackend();
        ~RaylibBackend() override;

        BackendType GetBackendType() const override { return BackendType::Raylib; }
    };

}  // namespace ugfx::raylib
#pragma once

#include <memory>

#include "../CommonTypes.h"
#include "IInput.h"
#include "IRenderer.h"
#include "IWindow.h"

namespace ugfx {

    class IGraphicsBackend {
       public:
        virtual ~IGraphicsBackend() = default;

        virtual IWindow*   GetWindow()   = 0;
        virtual IInput*    GetInput()    = 0;
        virtual IRenderer* GetRenderer() = 0;

        virtual BackendType GetBackendType() = 0;
    };

    std::unique_ptr<IGraphicsBackend> CreateBackend();

}  // namespace ugfx

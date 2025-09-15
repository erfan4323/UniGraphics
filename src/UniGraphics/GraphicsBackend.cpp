#include "GraphicsBackend.h"

namespace ugfx {

    GraphicsBackend::GraphicsBackend() = default;

    GraphicsBackend::~GraphicsBackend() {
        if (m_Window)
            m_Window->Shutdown();
    }

}  // namespace ugfx
#include "RaylibBackend.h"

#include "RaylibInput.h"
#include "RaylibRenderer.h"
#include "RaylibWindow.h"

namespace ugfx::raylib {

    RaylibBackend::RaylibBackend() {
        m_Window   = std::make_unique<RaylibWindow>();
        m_Input    = std::make_unique<RaylibInput>();
        m_Renderer = std::make_unique<RaylibRenderer>();
    }

    RaylibBackend::~RaylibBackend() {
        // Raylib cleanup handled in RaylibWindow::Shutdown
    }

}  // namespace ugfx::raylib

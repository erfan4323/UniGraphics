#include "GraphicsBackend.h"

GraphicsBackend::GraphicsBackend() = default;

GraphicsBackend::~GraphicsBackend() {
    if (m_Window)
        m_Window->Shutdown();
}
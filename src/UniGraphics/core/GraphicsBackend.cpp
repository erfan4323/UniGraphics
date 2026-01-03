#include "GraphicsBackend.h"

#ifdef USE_SDL
#include "../backends/sdl/SDLBackend.h"
#elif defined(USE_RAYLIB)
#include "../backends/raylib/RaylibBackend.h"
#endif

namespace ugfx {

    GraphicsBackend::GraphicsBackend() = default;

    GraphicsBackend::~GraphicsBackend() {
        if (m_Window)
            m_Window->Shutdown();
    }

    std::unique_ptr<IGraphicsBackend> CreateBackend() {
#ifdef USE_SDL
        return std::make_unique<sdl::SDLBackend>();
#elif defined(USE_RAYLIB)
        return std::make_unique<raylib::RaylibBackend>();
#else
        static_assert(false, "No backend defined! Define USE_SDL or USE_RAYLIB.");
        return nullptr;
#endif
    }

}  // namespace ugfx

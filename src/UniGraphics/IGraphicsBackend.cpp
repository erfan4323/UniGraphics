#include "IGraphicsBackend.h"

#include "backends/raylib/RaylibBackend.h"
#include "backends/sdl/SDLBackend.h"

namespace ugfx {

    std::unique_ptr<IGraphicsBackend> CreateBackend(BackendType type) {
        switch (type) {
            case BackendType::Raylib:
                return std::make_unique<ugfx::raylib::RaylibBackend>();
            case BackendType::SDL:
                return std::make_unique<ugfx::sdl::SDLBackend>();
            default:
                return nullptr;
        }
    }

}  // namespace ugfx
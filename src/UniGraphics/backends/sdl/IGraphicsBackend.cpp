#include "../../interfaces/IGraphicsBackend.h"

#include "SDLBackend.h"

namespace ugfx {

    std::unique_ptr<IGraphicsBackend> CreateBackend() {
        return std::make_unique<ugfx::sdl::SDLBackend>();
    }

}  // namespace ugfx

#include "../../interfaces/IGraphicsBackend.h"

#include "RaylibBackend.h"

namespace ugfx {

    std::unique_ptr<IGraphicsBackend> CreateBackend() {
        return std::make_unique<ugfx::raylib::RaylibBackend>();
    }

}  // namespace ugfx

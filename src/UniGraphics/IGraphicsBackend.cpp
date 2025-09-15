#include "IGraphicsBackend.h"

#include "backends/raylib/RaylibBackend.h"

namespace ugfx {

    std::unique_ptr<IGraphicsBackend> CreateBackend(BackendType type) {
        switch (type) {
            case BackendType::Raylib:
                return std::make_unique<ugfx::raylib::RaylibBackend>();
            default:
                return nullptr;
        }
    }

}  // namespace ugfx
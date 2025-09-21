#include "RaylibInput.h"

namespace ugfx::raylib {
    void* RaylibInput::GetHandle() const {
        return nullptr;
    }

    void RaylibInput::ProcessEvents(void* event) {
    }

    bool RaylibInput::IsKeyDown(ugfx::Key key) const {
        return ::IsKeyDown(MapKey(key));
    }

    bool RaylibInput::IsKeyPressed(ugfx::Key key) const {
        return ::IsKeyPressed(MapKey(key));
    }

    bool RaylibInput::IsKeyReleased(ugfx::Key key) const {
        return ::IsKeyReleased(MapKey(key));
    }

    bool RaylibInput::IsKeyUp(ugfx::Key key) const {
        return ::IsKeyUp(MapKey(key));
    }

    KeyboardKey RaylibInput::MapKey(ugfx::Key key) const {
        return static_cast<KeyboardKey>(static_cast<int>(key));
    }

}  // namespace ugfx::raylib
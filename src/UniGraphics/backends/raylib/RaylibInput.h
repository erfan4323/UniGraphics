#pragma once

#include <raylib.h>

#include "UniGraphics.h"

namespace ugfx::raylib {

    class RaylibInput : public IInput {
       public:
        RaylibInput()           = default;
        ~RaylibInput() override = default;

        void ProcessEvents(void* event) override;

        bool IsKeyDown(ugfx::Key key) const override;
        bool IsKeyPressed(ugfx::Key key) const override;
        bool IsKeyReleased(ugfx::Key key) const override;
        bool IsKeyUp(ugfx::Key key) const override;

       private:
        KeyboardKey MapKey(ugfx::Key key) const;
    };

}  // namespace ugfx::raylib
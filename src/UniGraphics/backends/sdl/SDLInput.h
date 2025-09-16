#pragma once

#include <SDL2/SDL.h>

#include <vector>

#include "UniGraphics.h"

namespace ugfx::sdl {

    class SDLInput : public IInput {
       public:
        SDLInput();
        ~SDLInput() override;

        bool IsKeyDown(ugfx::Key key) const override;
        bool IsKeyPressed(ugfx::Key key) const override;
        bool IsKeyReleased(ugfx::Key key) const override;
        bool IsKeyUp(ugfx::Key key) const override;

       private:
        mutable std::vector<SDL_Scancode> m_PressedKeys;
        mutable std::vector<SDL_Scancode> m_ReleasedKeys;

        SDL_Scancode MapKey(Key key) const;
    };

}  // namespace ugfx::sdl
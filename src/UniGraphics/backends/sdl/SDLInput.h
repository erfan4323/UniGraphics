#pragma once

#include <SDL2/SDL.h>

#include <bitset>
#include <vector>

#include "UniGraphics.h"

namespace ugfx::sdl {

    class SDLInput : public IInput {
       public:
        SDLInput();
        ~SDLInput() override;

        void ProcessEvents(void* event) override;

        bool IsKeyDown(ugfx::Key key) const override;
        bool IsKeyPressed(ugfx::Key key) const override;
        bool IsKeyReleased(ugfx::Key key) const override;
        bool IsKeyUp(ugfx::Key key) const override;

       private:
        std::bitset<SDL_NUM_SCANCODES> m_CurrentDown;
        std::bitset<SDL_NUM_SCANCODES> m_PressedThisFrame;
        std::bitset<SDL_NUM_SCANCODES> m_ReleasedThisFrame;

        SDL_Scancode MapKey(Key key) const;
    };

}  // namespace ugfx::sdl
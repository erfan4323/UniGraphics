#pragma once

#include <SDL2/SDL.h>

#include <bitset>
#include <functional>
#include <vector>

#include "UniGraphics.h"

namespace ugfx::sdl {

    class SDLInput : public IInput {
       public:
        SDLInput();
        ~SDLInput() override;

        void* GetHandle() const override;

        void ProcessEvents(void* event) override;
        void BeginFrame() override;
        void RegisterEventCallback(EventCallback callback) override;

        bool IsKeyDown(ugfx::Key key) const override;
        bool IsKeyPressed(ugfx::Key key) const override;
        bool IsKeyReleased(ugfx::Key key) const override;
        bool IsKeyUp(ugfx::Key key) const override;

       private:
        std::bitset<SDL_NUM_SCANCODES> m_CurrentDown;
        std::bitset<SDL_NUM_SCANCODES> m_PressedThisFrame;
        std::bitset<SDL_NUM_SCANCODES> m_ReleasedThisFrame;

        std::vector<EventCallback> m_EventCallbacks;

        SDL_Event m_LastEvent;

        SDL_Scancode MapKey(Key key) const;
    };

}  // namespace ugfx::sdl
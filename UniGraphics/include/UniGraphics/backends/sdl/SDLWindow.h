#pragma once

#include <SDL2/SDL.h>

#include "UniGraphics.h"

namespace ugfx::sdl {

    class SDLWindow : public IWindow {
       public:
        // SDLWindow();
        explicit SDLWindow(IInput* input);
        ~SDLWindow() override;

        bool                Create(const std::string& title, int width, int height, WindowFlags flags) override;
        void                SetTitle(const std::string& title) override;
        std::pair<int, int> GetSize() const override;
        bool                ShouldClose() const override;
        void                PollEvents() override;
        void                Shutdown() override;
        void                SetTargetFPS(int fps) override;
        float               GetDeltaTime() const override;
        uint32_t            GetTicks() const override;

        SDL_Window* GetWindow() const { return m_Window; }

       private:
        SDL_Window* m_Window          = nullptr;
        bool        m_ShouldClose     = false;
        Uint32      m_LastFrameTime   = 0;
        float       m_TargetFrameTime = 0.0f;

        int m_CachedWidth  = 0;
        int m_CachedHeight = 0;

        IInput* m_Input = nullptr;
    };

}  // namespace ugfx::sdl
#include "SDLWindow.h"

namespace ugfx::sdl {

    // SDLWindow::SDLWindow() {
    // }

    SDLWindow::SDLWindow(IInput* input) {
        m_Window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 10, 10, 0);
        m_Input  = input;
    }

    SDLWindow::~SDLWindow() {
        Shutdown();
    }

    bool SDLWindow::Create(const std::string& title, int width, int height, WindowFlags flags) {
        Uint32 sdlFlags = SDL_WINDOW_SHOWN;

        if (HasFlag(flags, WindowFlags::Fullscreen))
            sdlFlags |= SDL_WINDOW_FULLSCREEN;
        if (HasFlag(flags, WindowFlags::Borderless))
            sdlFlags |= SDL_WINDOW_BORDERLESS;
        if (HasFlag(flags, WindowFlags::Resizable))
            sdlFlags |= SDL_WINDOW_RESIZABLE;
        if (HasFlag(flags, WindowFlags::Hidden))
            sdlFlags |= SDL_WINDOW_HIDDEN;
        if (HasFlag(flags, WindowFlags::AlwaysOnTop))
            sdlFlags |= SDL_WINDOW_ALWAYS_ON_TOP;

        // If window exists, update or recreate
        if (m_Window) {
            SDL_SetWindowTitle(m_Window, title.c_str());
            SDL_SetWindowSize(m_Window, width, height);
            SDL_SetWindowPosition(m_Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

            if (HasFlag(flags, WindowFlags::Fullscreen)) {
                SDL_SetWindowFullscreen(m_Window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            } else {
                SDL_SetWindowFullscreen(m_Window, 0);
            }
            SDL_SetWindowBordered(m_Window, HasFlag(flags, WindowFlags::Borderless) ? SDL_FALSE : SDL_TRUE);
            SDL_SetWindowAlwaysOnTop(m_Window, HasFlag(flags, WindowFlags::AlwaysOnTop) ? SDL_TRUE : SDL_FALSE);
            SDL_SetWindowResizable(m_Window, HasFlag(flags, WindowFlags::Resizable) ? SDL_TRUE : SDL_FALSE);
            return true;
        }

        m_Window =
            SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, sdlFlags);
        m_LastFrameTime = SDL_GetTicks();
        return m_Window != nullptr;
    }

    void SDLWindow::SetTitle(const std::string& title) {
        if (m_Window)
            SDL_SetWindowTitle(m_Window, title.c_str());
    }

    std::pair<int, int> SDLWindow::GetSize() const {
        int w, h;
        SDL_GetWindowSize(m_Window, &w, &h);
        return {w, h};
    }

    bool SDLWindow::ShouldClose() const {
        return m_ShouldClose;
    }

    void SDLWindow::PollEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                m_ShouldClose = true;

            m_Input->ProcessEvents(&event);
        }

        if (m_TargetFrameTime > 0.0f) {
            Uint32 currentTime = SDL_GetTicks();
            float  elapsed     = (currentTime - m_LastFrameTime) / 1000.0f;
            if (elapsed < m_TargetFrameTime) {
                SDL_Delay(static_cast<Uint32>((m_TargetFrameTime - elapsed) * 1000.0f));
            }
            m_LastFrameTime = SDL_GetTicks();
        }
    }

    void SDLWindow::Shutdown() {
        if (m_Window) {
            SDL_DestroyWindow(m_Window);
            m_Window = nullptr;
        }
    }

    void SDLWindow::SetTargetFPS(int fps) {
        if (fps > 0) {
            m_TargetFrameTime = 1.0f / fps;
        } else {
            m_TargetFrameTime = 0.0f;
        }
    }

    float SDLWindow::GetDeltaTime() const {
        static Uint32 lastTime    = SDL_GetTicks();
        Uint32        currentTime = SDL_GetTicks();
        float         deltaTime   = (currentTime - lastTime) / 1000.0f;
        lastTime                  = currentTime;
        return deltaTime;
    }

    uint32_t SDLWindow::GetTicks() const {
        return SDL_GetTicks();
    }

}  // namespace ugfx::sdl
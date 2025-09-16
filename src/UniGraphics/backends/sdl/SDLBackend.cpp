#include "SDLBackend.h"

#include <SDL2/SDL.h>

#include <iostream>

#include "SDLInput.h"
#include "SDLRenderer.h"
#include "SDLWindow.h"

namespace ugfx::sdl {

    SDLBackend::SDLBackend() {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
            std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
            return;
        }
        SDL_version version;
        SDL_GetVersion(&version);
        std::cout << "SDL Version: " << (int) version.major << "." << (int) version.minor << "." << (int) version.patch
                  << std::endl;

        m_Window = std::make_unique<SDLWindow>();
        if (!m_Window) {
            std::cerr << "Failed to create SDLWindow" << std::endl;
            return;
        }

        // // Create renderer only if window is valid
        auto* sdlWindow = static_cast<SDLWindow*>(m_Window.get());
        if (!sdlWindow->GetWindow()) {
            std::cerr << "SDLWindow has no valid window" << std::endl;
            return;
        }
        m_Renderer = std::make_unique<SDLRenderer>(sdlWindow->GetWindow());
        if (!m_Renderer) {
            std::cerr << "Failed to create SDLRenderer" << std::endl;
            return;
        }
        m_Input = std::make_unique<SDLInput>();
    }

    SDLBackend::~SDLBackend() {
        m_Renderer.reset();  // Destroy renderer first
        m_Window.reset();
        m_Input.reset();
        SDL_Quit();
    }

}  // namespace ugfx::sdl
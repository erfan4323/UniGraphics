#pragma once

#include <SDL2/SDL.h>

#include "UniGraphics.h"

namespace ugfx::sdl {

    class SDLRenderer : public IRenderer {
       public:
        explicit SDLRenderer(SDL_Window* window);
        ~SDLRenderer() override;

        // IRenderer methods
        void BeginDrawing() override;
        void EndDrawing() override;
        void Clear(ugfx::Color color) override;

        void DrawPixel(ugfx::Vector2 pos, ugfx::Color color) override;
        void DrawLine(ugfx::Vector2 start, ugfx::Vector2 end, float thickness, ugfx::Color color) override;
        void DrawRectangle(ugfx::Rectangle rec, ugfx::Color color) override;
        void DrawRectangleLines(ugfx::Rectangle rec, float thickness, ugfx::Color color) override;
        void DrawCircle(ugfx::Vector2 center, float radius, ugfx::Color color) override;
        void DrawTriangle(ugfx::Vector2 v1, ugfx::Vector2 v2, ugfx::Vector2 v3, ugfx::Color color) override;

       private:
        SDL_Renderer* m_Renderer = nullptr;
    };

}  // namespace ugfx::sdl
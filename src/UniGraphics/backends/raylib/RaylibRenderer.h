#pragma once

#include <raylib.h>

#include "UniGraphics.h"

namespace ugfx::raylib {

    class RaylibRenderer : public IRenderer {
       public:
        RaylibRenderer();
        ~RaylibRenderer() override;

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
    };

}  // namespace ugfx::raylib
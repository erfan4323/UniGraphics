#pragma once

#include <string>

#include "CommonTypes.h"

namespace ugfx {

    class IRenderer {
       public:
        virtual ~IRenderer() = default;

        virtual void BeginDrawing()           = 0;
        virtual void EndDrawing()             = 0;
        virtual void Clear(ugfx::Color color) = 0;

        // Basic primitives
        virtual void DrawPixel(ugfx::Vector2 pos, ugfx::Color color)                                       = 0;
        virtual void DrawLine(ugfx::Vector2 start, ugfx::Vector2 end, float thickness, ugfx::Color color)  = 0;
        virtual void DrawRectangle(ugfx::Rectangle rec, ugfx::Color color)                                 = 0;
        virtual void DrawRectangleLines(ugfx::Rectangle rec, float thickness, ugfx::Color color)           = 0;
        virtual void DrawCircle(ugfx::Vector2 center, float radius, ugfx::Color color)                     = 0;
        virtual void DrawTriangle(ugfx::Vector2 v1, ugfx::Vector2 v2, ugfx::Vector2 v3, ugfx::Color color) = 0;
    };

}  // namespace ugfx
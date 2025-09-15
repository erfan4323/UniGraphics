#pragma once

#include <string>

#include "CommonTypes.h"

class IRenderer {
   public:
    virtual ~IRenderer() = default;

    virtual void BeginDrawing()     = 0;
    virtual void EndDrawing()       = 0;
    virtual void Clear(Color color) = 0;

    // Basic primitives
    virtual void DrawPixel(Vector2 pos, Color color)                                         = 0;
    virtual void DrawLine(Vector2 start, Vector2 end, float thickness, Color color)          = 0;
    virtual void DrawRectangle(Rectangle rec, Color color)                                   = 0;
    virtual void DrawRectangleLines(Rectangle rec, float thickness, Color color)             = 0;
    virtual void DrawCircle(Vector2 center, float radius, Color color)                       = 0;
    virtual void DrawCircleLines(Vector2 center, float radius, float thickness, Color color) = 0;
    virtual void DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color)               = 0;
};
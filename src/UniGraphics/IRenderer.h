#pragma once

#include <string>

#include "CommonTypes.h"

namespace ugfx {

    class IShapeRenderer {
       public:
        virtual ~IShapeRenderer() = default;

        virtual void DrawPixel(Vector2 pos, Color color)                                = 0;
        virtual void DrawLine(Vector2 start, Vector2 end, float thickness, Color color) = 0;
        virtual void DrawRectangle(Rectangle rec, Color color)                          = 0;
        virtual void DrawRectangleLines(Rectangle rec, float thickness, Color color)    = 0;
        virtual void DrawCircle(Vector2 center, float radius, Color color)              = 0;
        virtual void DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color)      = 0;
    };

    class IImageRenderer {
       public:
        virtual ~IImageRenderer()                                                                = default;
        virtual Texture LoadTexture(const std::string& path)                                     = 0;
        virtual void    UnloadTexture(Texture tex)                                               = 0;
        virtual void    DrawTexture(Texture tex, Vector2 pos, Color tint = {255, 255, 255, 255}) = 0;
        virtual void DrawTextureRegion(Texture tex, Rectangle src, Vector2 dst, Color tint = {255, 255, 255, 255}) = 0;
        virtual void DrawTextureRegion(Texture texture, Rectangle src, Rectangle dest, Vector2 origin, float rotation,
                                       Flip flip, Color tint = {255, 255, 255, 255})                               = 0;
        virtual void DrawTextureEx(Texture tex, Vector2 pos, Vector2 origin, float rotation, float scale, Flip flip,
                                   Color tint = {255, 255, 255, 255})                                              = 0;
    };

    class ITextRenderer {
       public:
        virtual ~ITextRenderer() = default;

        virtual Font LoadFont(const std::string& path, int size)                               = 0;
        virtual void UnloadFont(Font font)                                                     = 0;
        virtual void DrawText(Font font, const std::string& text, Vector2 pos, Color color)    = 0;
        virtual void DrawText(const std::string& text, Vector2 pos, int fontSize, Color color) = 0;
    };

    class IRenderer : public IShapeRenderer, public IImageRenderer, public ITextRenderer {
       public:
        virtual ~IRenderer() = default;

        virtual void BeginDrawing()        = 0;
        virtual void EndDrawing()          = 0;
        virtual void Clear(Color color)    = 0;
        virtual void ReleaseAllResources() = 0;
        virtual void* GetHandle() const     = 0;
    };

}  // namespace ugfx
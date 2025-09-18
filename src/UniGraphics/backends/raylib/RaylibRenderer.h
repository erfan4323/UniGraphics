#pragma once

#include <raylib.h>

#include "UniGraphics.h"

namespace ugfx::raylib {

    class RaylibRenderer : public IRenderer {
       public:
        RaylibRenderer();
        ~RaylibRenderer() override;

        // IRenderer
        void BeginDrawing() override;
        void EndDrawing() override;
        void Clear(Color color) override;

        // IShapeRenderer
        void DrawPixel(Vector2 pos, Color color) override;
        void DrawLine(Vector2 start, Vector2 end, float thickness, Color color) override;
        void DrawRectangle(Rectangle rec, Color color) override;
        void DrawRectangleLines(Rectangle rec, float thickness, Color color) override;
        void DrawCircle(Vector2 center, float radius, Color color) override;
        void DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color) override;

        // IImageRenderer
        Texture LoadTexture(const std::string& path) override;
        void    UnloadTexture(Texture tex) override;
        void    DrawTexture(Texture tex, Vector2 pos, Color tint) override;
        void    DrawTextureRegion(Texture tex, Rectangle src, Vector2 dst, Color tint) override;
        void    DrawTextureEx(Texture tex, Vector2 pos, Vector2 origin, float rotation, float scale, Flip flip,
                              Color tint) override;

        // ITextRenderer
        Font LoadFont(const std::string& path, int size) override;
        void UnloadFont(Font font) override;
        void DrawText(Font font, const std::string& text, Vector2 pos, Color color) override;
        void DrawText(const std::string& text, Vector2 pos, int fontSize, Color color) override;

       private:
        ::Font m_DefaultFont = {0};
    };

}  // namespace ugfx::raylib
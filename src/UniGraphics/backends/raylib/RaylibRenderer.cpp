#include "RaylibRenderer.h"

#include <functional>
#include <iostream>

#include "RaylibConverter.h"

namespace ugfx::raylib {

    RaylibRenderer::RaylibRenderer() {
        m_DefaultFont = GetFontDefault();
    }

    RaylibRenderer::~RaylibRenderer() {
        ::UnloadFont(m_DefaultFont);
    }

    void RaylibRenderer::BeginDrawing() {
        ::BeginDrawing();
    }

    void RaylibRenderer::EndDrawing() {
        ::EndDrawing();
    }

    void RaylibRenderer::Clear(ugfx::Color color) {
        ::ClearBackground(ugfx::raylib::ToRaylib(color));
    }

    void RaylibRenderer::ReleaseAllResources() {
        std::cout << "RaylibRenderer: All textures/fonts released.\n";
    }

    void RaylibRenderer::DrawPixel(ugfx::Vector2 pos, ugfx::Color color) {
        ::DrawPixelV(ugfx::raylib::ToRaylib(pos), ugfx::raylib::ToRaylib(color));
    }

    void RaylibRenderer::DrawLine(ugfx::Vector2 start, ugfx::Vector2 end, float thickness, ugfx::Color color) {
        ::DrawLineEx(ugfx::raylib::ToRaylib(start), ugfx::raylib::ToRaylib(end), thickness,
                     ugfx::raylib::ToRaylib(color));
    }

    void RaylibRenderer::DrawRectangle(ugfx::Rectangle rect, ugfx::Color color) {
        ::DrawRectangleRec(ugfx::raylib::ToRaylib(rect), ugfx::raylib::ToRaylib(color));
    }

    void RaylibRenderer::DrawRectangleLines(ugfx::Rectangle rect, float thickness, ugfx::Color color) {
        ::DrawRectangleLinesEx(ugfx::raylib::ToRaylib(rect), thickness, ugfx::raylib::ToRaylib(color));
    }

    void RaylibRenderer::DrawCircle(ugfx::Vector2 center, float radius, ugfx::Color color) {
        ::DrawCircleV(ugfx::raylib::ToRaylib(center), radius, ugfx::raylib::ToRaylib(color));
    }

    void RaylibRenderer::DrawTriangle(ugfx::Vector2 v1, ugfx::Vector2 v2, ugfx::Vector2 v3, ugfx::Color color) {
        ::DrawTriangle(ugfx::raylib::ToRaylib(v1), ugfx::raylib::ToRaylib(v2), ugfx::raylib::ToRaylib(v3),
                       ugfx::raylib::ToRaylib(color));
    }

    Texture RaylibRenderer::LoadTexture(const std::string& path) {
        ::Texture2D tex = ::LoadTexture(path.c_str());
        if (tex.id == 0) {
            std::cerr << "Failed to load texture: " << path << std::endl;
            return Texture{-1};
        }
        return Texture{static_cast<int>(tex.id), tex.width, tex.height};
    }

    void RaylibRenderer::UnloadTexture(Texture tex) {
        if (tex.id != -1)
            ::UnloadTexture(ugfx::raylib::ToRaylib(tex));
    }

    void RaylibRenderer::DrawTexture(Texture tex, Vector2 pos, Color tint) {
        if (tex.id == -1)
            return;
        ::DrawTextureV(ugfx::raylib::ToRaylib(tex), ugfx::raylib::ToRaylib(pos), ugfx::raylib::ToRaylib(tint));
    }

    void RaylibRenderer::DrawTextureRegion(Texture tex, Rectangle src, Vector2 dst, Color tint) {
        if (tex.id == -1)
            return;
        ::Rectangle rSrc = ugfx::raylib::ToRaylib(src);
        ::Rectangle rDst = ugfx::raylib::ToRaylib(src);
        ::DrawTextureRec(ugfx::raylib::ToRaylib(tex), rSrc, ugfx::raylib::ToRaylib(dst), ugfx::raylib::ToRaylib(tint));
    }

    void RaylibRenderer::DrawTextureEx(Texture tex, Vector2 pos, Vector2 origin, float rotation, float scale, Flip flip,
                                       Color tint) {
        if (tex.id == -1)
            return;

        ::Rectangle rSrc = {0, 0, static_cast<float>(tex.width), static_cast<float>(tex.height)};
        ::Rectangle rDst = {pos.x, pos.y, tex.width * scale, tex.height * scale};

        ::Vector2 rOrigin = {origin.x * scale, origin.y * scale};

        float finalScaleX = scale;
        float finalScaleY = scale;
        if (flip == Flip::Horizontal)
            finalScaleX = -scale;
        if (flip == Flip::Vertical)
            finalScaleY = -scale;

        rDst.width  = tex.width * std::abs(finalScaleX);
        rDst.height = tex.height * std::abs(finalScaleY);

        ::DrawTexturePro(ugfx::raylib::ToRaylib(tex), rSrc, rDst, rOrigin, rotation, ugfx::raylib::ToRaylib(tint));
    }

    Font RaylibRenderer::LoadFont(const std::string& path, int size) {
        ::Font font = LoadFontEx(path.c_str(), size, nullptr, 0);
        if (font.texture.id == 0) {
            std::cerr << "Failed to load font: " << path << std::endl;
            return Font{-1};
        }
        return Font{static_cast<int>(font.texture.id)};
    }

    void RaylibRenderer::UnloadFont(Font font) {
        if (font.id != -1)
            ::UnloadFont({font.id});
    }

    void RaylibRenderer::DrawText(Font font, const std::string& text, Vector2 pos, Color color) {
        ::Font rFont = (font.id != -1) ? ::Font{font.id} : m_DefaultFont;
        ::DrawTextEx(rFont, text.c_str(), {pos.x, pos.y}, rFont.baseSize, 1.0f, {color.r, color.g, color.b, color.a});
    }

    void RaylibRenderer::DrawText(const std::string& text, Vector2 pos, int fontSize, Color color) {
        ::DrawTextEx(m_DefaultFont, text.c_str(), {pos.x, pos.y}, fontSize, 1.0f, {color.r, color.g, color.b, color.a});
    }

}  // namespace ugfx::raylib
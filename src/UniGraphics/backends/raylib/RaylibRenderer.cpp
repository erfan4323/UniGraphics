#include "RaylibRenderer.h"

#include <functional>
#include <iostream>

#include "RaylibConverter.h"

namespace ugfx::raylib {

    RaylibRenderer::RaylibRenderer() {
    }

    RaylibRenderer::~RaylibRenderer() {
        ReleaseAllResources();
    }

    void RaylibRenderer::BeginDrawing() {
        ::BeginDrawing();
    }

    void RaylibRenderer::EndDrawing() {
        ::EndDrawing();
    }

    void RaylibRenderer::Clear(ugfx::Color color) {
        ::ClearBackground(ToRaylib(color));
    }

    void RaylibRenderer::ReleaseAllResources() {
        m_FontManager.Clear([](::Font* f) {
            ::UnloadFont(*f);
            delete f;
        });
        m_TextureManager.Clear([](::Texture2D* t) {
            ::UnloadTexture(*t);
            delete t;
        });
        std::cout << "RaylibRenderer: All textures/fonts released.\n";
    }

    void* RaylibRenderer::GetHandle() const {
        return nullptr;
    }

    void RaylibRenderer::DrawPixel(ugfx::Vector2 pos, ugfx::Color color) {
        ::DrawPixelV(ToRaylib(pos), ToRaylib(color));
    }

    void RaylibRenderer::DrawLine(ugfx::Vector2 start, ugfx::Vector2 end, float thickness, ugfx::Color color) {
        ::DrawLineEx(ToRaylib(start), ToRaylib(end), thickness, ToRaylib(color));
    }

    void RaylibRenderer::DrawRectangle(ugfx::Rectangle rect, ugfx::Color color) {
        ::DrawRectangleRec(ToRaylib(rect), ToRaylib(color));
    }

    void RaylibRenderer::DrawRectangleLines(ugfx::Rectangle rect, float thickness, ugfx::Color color) {
        ::DrawRectangleLinesEx(ToRaylib(rect), thickness, ToRaylib(color));
    }

    void RaylibRenderer::DrawCircle(ugfx::Vector2 center, float radius, ugfx::Color color) {
        ::DrawCircleV(ToRaylib(center), radius, ToRaylib(color));
    }

    void RaylibRenderer::DrawTriangle(ugfx::Vector2 v1, ugfx::Vector2 v2, ugfx::Vector2 v3, ugfx::Color color) {
        ::DrawTriangle(ToRaylib(v1), ToRaylib(v2), ToRaylib(v3), ToRaylib(color));
    }

    Texture RaylibRenderer::LoadTexture(const std::string& path) {
        ::Texture2D* t = new ::Texture2D(::LoadTexture(path.c_str()));
        if (t->id == 0) {
            std::cerr << "Failed to load texture: " << path << std::endl;
            delete t;
            return Texture{-1};
        }
        int id = m_TextureManager.Add(t);
        return Texture{id, t->width, t->height};
    }

    void RaylibRenderer::UnloadTexture(Texture tex) {
        ::Texture2D* t = m_TextureManager.Get(tex.id);
        if (t) {
            ::UnloadTexture(*t);
            delete t;
            m_TextureManager.Remove(tex.id);
        }
    }

    void RaylibRenderer::DrawTexture(Texture tex, Vector2 pos, Color tint) {
        if (tex.id == -1)
            return;

        ::Texture2D* texture = m_TextureManager.Get(tex.id);
        if (!texture)
            return;
        ::DrawTextureV(*texture, ToRaylib(pos), ToRaylib(tint));
    }

    void RaylibRenderer::DrawTextureRegion(Texture tex, Rectangle src, Vector2 dst, Color tint) {
        if (tex.id == -1)
            return;

        ::Texture2D* texture = m_TextureManager.Get(tex.id);
        if (!texture)
            return;

        ::Rectangle rSrc = ToRaylib(src);
        ::Vector2   rDst = ToRaylib(dst);
        ::DrawTextureRec(*texture, rSrc, rDst, ToRaylib(tint));
    }

    void RaylibRenderer::DrawTextureRegion(Texture texture, Rectangle src, Rectangle dest, Vector2 origin,
                                           float rotation, Flip flip, Color tint) {
        if (texture.id == -1)
            return;
        ::Texture2D* tex = m_TextureManager.Get(texture.id);
        if (!tex)
            return;

        ::Rectangle srcRect = {src.x, src.y, src.width, src.height};
        ::Rectangle dstRect = {dest.x, dest.y, dest.width, dest.height};
        ::Vector2   rOrigin = {origin.x, origin.y};

        float finalScaleX = dest.width / src.width;
        float finalScaleY = dest.height / src.height;
        if (flip == Flip::Horizontal || flip == Flip::Both)
            finalScaleX = -finalScaleX;
        if (flip == Flip::Vertical || flip == Flip::Both)
            finalScaleY = -finalScaleY;

        dstRect.width  = src.width * std::abs(finalScaleX);
        dstRect.height = src.height * std::abs(finalScaleY);

        ::DrawTexturePro(*tex, srcRect, dstRect, rOrigin, rotation, ToRaylib(tint));
    }

    void RaylibRenderer::DrawTextureEx(Texture tex, Vector2 pos, Vector2 origin, float rotation, float scale, Flip flip,
                                       Color tint) {
        if (tex.id == -1)
            return;

        ::Texture2D* texture = m_TextureManager.Get(tex.id);
        if (!texture)
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

        ::DrawTexturePro(*texture, rSrc, rDst, rOrigin, rotation, ToRaylib(tint));
    }

    Font RaylibRenderer::LoadFont(const std::string& path, int size) {
        ::Font* f = new ::Font(LoadFontEx(path.c_str(), size, nullptr, 0));
        if (f->texture.id == 0) {
            std::cerr << "Failed to load font: " << path << std::endl;
            delete f;
            return Font{-1};
        }
        int id = m_FontManager.Add(f);
        return Font{id};
    }

    void RaylibRenderer::UnloadFont(Font font) {
        ::Font* f = m_FontManager.Get(font.id);
        if (f) {
            ::UnloadFont(*f);
            delete f;
            m_FontManager.Remove(font.id);
        }
    }

    void RaylibRenderer::DrawText(Font font, const std::string& text, Vector2 pos, Color color) {
        auto f = resolveFont(font);
        ::DrawTextEx(f, text.c_str(), ToRaylib(pos), f.baseSize, 1.0f, ToRaylib(color));
    }

    void RaylibRenderer::DrawText(const std::string& text, Vector2 pos, int fontSize, Color color) {
        ::DrawTextEx(defaultFont(), text.c_str(), ToRaylib(pos), fontSize, 1.0f, ToRaylib(color));
    }

}  // namespace ugfx::raylib
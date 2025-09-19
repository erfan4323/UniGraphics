#include "SDLRenderer.h"

#include <cmath>
#include <functional>
#include <iostream>

#include "Lexend.h"

#define M_PI 3.14159265358979323846  // pi

namespace ugfx::sdl {

    SDLRenderer::SDLRenderer(SDL_Window* window) {
        if (!window) {
            std::cerr << "SDL_Window is null in SDLRenderer constructor" << std::endl;
            return;
        }
        m_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!m_Renderer) {
            std::cerr << "SDL_CreateRenderer (accelerated) failed: " << SDL_GetError() << std::endl;
            m_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
            if (!m_Renderer) {
                std::cerr << "SDL_CreateRenderer (software) failed: " << SDL_GetError() << std::endl;
                return;
            }
            std::cout << "Using software renderer as fallback" << std::endl;
        }
        SDL_SetRenderDrawBlendMode(m_Renderer, SDL_BLENDMODE_BLEND);

        // Create RWops from memory
        SDL_RWops* rw = SDL_RWFromConstMem(Lexend_ttf, Lexend_ttf_len);
        if (!rw) {
            std::cerr << "SDL_RWFromConstMem failed\n";
            return;
        }

        // Open font from RWops, 16px default size
        m_DefaultFont = TTF_OpenFontRW(rw, 1, 16);  // 1 = auto-free RWops
        if (!m_DefaultFont) {
            std::cerr << "Failed to load embedded default font: " << TTF_GetError() << "\n";
        }
    }

    SDLRenderer::~SDLRenderer() {
        if (m_DefaultFont)
            TTF_CloseFont(m_DefaultFont);
        if (m_Renderer)
            SDL_DestroyRenderer(m_Renderer);

        ReleaseAllResources();
    }

    void SDLRenderer::BeginDrawing() {
        if (!m_Renderer) {
            std::cerr << "Renderer is null in BeginDrawing" << std::endl;
        }
    }

    void SDLRenderer::EndDrawing() {
        if (m_Renderer)
            SDL_RenderPresent(m_Renderer);
    }

    void SDLRenderer::Clear(Color color) {
        if (!m_Renderer)
            return;
        SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
        SDL_RenderClear(m_Renderer);
    }

    void SDLRenderer::ReleaseAllResources() {
        m_TextureManager.Clear([](SDL_Texture* t) { SDL_DestroyTexture(t); });
        m_FontManager.Clear([](TTF_Font* f) { TTF_CloseFont(f); });
        if (m_Renderer)
            SDL_RenderClear(m_Renderer);
        std::cout << "SDLRenderer: All textures/fonts released.\n";
    }

    void SDLRenderer::DrawPixel(Vector2 pos, Color color) {
        if (!m_Renderer)
            return;
        SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawPointF(m_Renderer, pos.x, pos.y);
    }

    void SDLRenderer::DrawLine(Vector2 start, Vector2 end, float thickness, Color color) {
        if (!m_Renderer)
            return;

        SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);

        if (thickness <= 1.0f) {
            SDL_RenderDrawLineF(m_Renderer, start.x, start.y, end.x, end.y);
            return;
        }

        // Direction vector
        float dx     = end.x - start.x;
        float dy     = end.y - start.y;
        float length = sqrtf(dx * dx + dy * dy);
        if (length < 0.001f)
            return;

        // Normalize perpendicular vector
        float px = -dy / length * (thickness / 2.0f);
        float py = dx / length * (thickness / 2.0f);

        // Build quad (two triangles)
        Vector2 v1 = {start.x + px, start.y + py};
        Vector2 v2 = {start.x - px, start.y - py};
        Vector2 v3 = {end.x + px, end.y + py};
        Vector2 v4 = {end.x - px, end.y - py};

        // Fill triangles (your DrawTriangle from before!)
        DrawTriangle(v1, v2, v3, color);
        DrawTriangle(v2, v3, v4, color);
    }

    void SDLRenderer::DrawRectangle(Rectangle rec, Color color) {
        if (!m_Renderer) {
            std::cerr << "Renderer is null in DrawRectangle" << std::endl;
            return;
        }
        SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
        SDL_FRect rect = {rec.x, rec.y, rec.width, rec.height};
        SDL_RenderFillRectF(m_Renderer, &rect);
    }

    void SDLRenderer::DrawRectangleLines(Rectangle rec, float thickness, Color color) {
        if (!m_Renderer)
            return;

        SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);

        SDL_FRect top    = {rec.x, rec.y, rec.width, thickness};
        SDL_FRect bottom = {rec.x, rec.y + rec.height - thickness, rec.width, thickness};
        SDL_FRect left   = {rec.x, rec.y, thickness, rec.height};
        SDL_FRect right  = {rec.x + rec.width - thickness, rec.y, thickness, rec.height};

        SDL_RenderFillRectF(m_Renderer, &top);
        SDL_RenderFillRectF(m_Renderer, &bottom);
        SDL_RenderFillRectF(m_Renderer, &left);
        SDL_RenderFillRectF(m_Renderer, &right);
    }

    void SDLRenderer::DrawCircle(Vector2 center, float radius, Color color) {
        if (!m_Renderer)
            return;

        SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);

        int yStart = static_cast<int>(center.y - radius);
        int yEnd   = static_cast<int>(center.y + radius);

        for (int y = yStart; y <= yEnd; ++y) {
            float dy     = y + 0.5f - center.y;
            float dx     = sqrtf(radius * radius - dy * dy);  // half-width of the line at this y
            float xStart = center.x - dx;
            float xEnd   = center.x + dx;
            SDL_RenderDrawLineF(m_Renderer, xStart, static_cast<float>(y), xEnd, static_cast<float>(y));
        }
    }

    void SDLRenderer::DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color) {
        if (!m_Renderer)
            return;

        // Convert to integer for pixel rasterization
        struct Point {
            int x, y;
        };
        Point p1{static_cast<int>(v1.x), static_cast<int>(v1.y)};
        Point p2{static_cast<int>(v2.x), static_cast<int>(v2.y)};
        Point p3{static_cast<int>(v3.x), static_cast<int>(v3.y)};

        // Sort vertices by y-coordinate ascending (p1.y <= p2.y <= p3.y)
        if (p2.y < p1.y)
            std::swap(p1, p2);
        if (p3.y < p1.y)
            std::swap(p1, p3);
        if (p3.y < p2.y)
            std::swap(p2, p3);

        auto drawSpan = [&](int y, int x1, int x2) {
            if (x1 > x2)
                std::swap(x1, x2);
            SDL_RenderDrawLine(m_Renderer, x1, y, x2, y);
        };

        SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);

        auto edgeInterp = [](Point a, Point b, int y) {
            if (a.y == b.y)
                return a.x;
            return a.x + (b.x - a.x) * (y - a.y) / (b.y - a.y);
        };

        // Fill bottom half (p1 -> p2, p1 -> p3)
        for (int y = p1.y; y <= p2.y; ++y) {
            int x1 = edgeInterp(p1, p2, y);
            int x2 = edgeInterp(p1, p3, y);
            drawSpan(y, x1, x2);
        }

        // Fill top half (p2 -> p3, p1 -> p3)
        for (int y = p2.y; y <= p3.y; ++y) {
            int x1 = edgeInterp(p2, p3, y);
            int x2 = edgeInterp(p1, p3, y);
            drawSpan(y, x1, x2);
        }
    }

    Texture SDLRenderer::LoadTexture(const std::string& path) {
        if (!m_Renderer)
            return {0, 0, 0};

        SDL_Texture* tex = IMG_LoadTexture(m_Renderer, path.c_str());
        if (!tex) {
            std::cerr << "Failed to load texture: " << IMG_GetError() << std::endl;
            return {0, 0, 0};
        }

        int w, h;
        SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
        unsigned int id = m_TextureManager.Add(tex);
        return {static_cast<int>(id), w, h};
    }

    void SDLRenderer::UnloadTexture(Texture tex) {
        SDL_Texture* t = m_TextureManager.Get(tex.id);
        if (t) {
            SDL_DestroyTexture(t);
            m_TextureManager.Remove(tex.id);
        }
    }

    void SDLRenderer::DrawTexture(Texture tex, Vector2 pos, Color tint) {
        if (!m_Renderer || tex.id == 0)
            return;

        SDL_Texture* realTex = m_TextureManager.Get(tex.id);

        SDL_SetTextureColorMod(realTex, tint.r, tint.g, tint.b);
        SDL_SetTextureAlphaMod(realTex, tint.a);

        SDL_FRect dst = {pos.x, pos.y, static_cast<float>(tex.width), static_cast<float>(tex.height)};
        SDL_RenderCopyF(m_Renderer, realTex, nullptr, &dst);
    }

    void SDLRenderer::DrawTextureRegion(Texture tex, Rectangle src, Vector2 dst, Color tint) {
        if (!m_Renderer || tex.id == 0)
            return;

        SDL_Texture* realTex = m_TextureManager.Get(tex.id);

        SDL_SetTextureColorMod(realTex, tint.r, tint.g, tint.b);
        SDL_SetTextureAlphaMod(realTex, tint.a);

        SDL_Rect  srcRect = {static_cast<int>(src.x), static_cast<int>(src.y), static_cast<int>(src.width),
                             static_cast<int>(src.height)};
        SDL_FRect dstRect = {dst.x, dst.y, src.width, src.height};
        SDL_RenderCopyF(m_Renderer, realTex, &srcRect, &dstRect);
    }

    void SDLRenderer::DrawTextureEx(Texture tex, Vector2 pos, Vector2 origin, float rotation, float scale, Flip flip,
                                    Color tint) {
        if (!m_Renderer || tex.id == 0)
            return;
        SDL_Texture* realTex = m_TextureManager.Get(tex.id);
        SDL_SetTextureColorMod(realTex, tint.r, tint.g, tint.b);
        SDL_SetTextureAlphaMod(realTex, tint.a);

        SDL_FRect dst = {pos.x - (origin.x * scale), pos.y - (origin.y * scale), tex.width * scale, tex.height * scale};
        SDL_FPoint center = {origin.x * scale, origin.y * scale};

        SDL_RendererFlip sdlFlip = SDL_FLIP_NONE;
        if (flip == Flip::Horizontal)
            sdlFlip = SDL_FLIP_HORIZONTAL;
        else if (flip == Flip::Vertical)
            sdlFlip = SDL_FLIP_VERTICAL;
        SDL_RenderCopyExF(m_Renderer, realTex, nullptr, &dst, rotation, &center, sdlFlip);
    }

    Font SDLRenderer::LoadFont(const std::string& path, int size) {
        TTF_Font* font = TTF_OpenFont(path.c_str(), size);
        if (!font) {
            std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
            return {0};
        }
        int id = m_FontManager.Add(font);
        return {id};
    }

    void SDLRenderer::UnloadFont(Font font) {
        TTF_Font* f = m_FontManager.Get(font.id);
        if (f) {
            TTF_CloseFont(f);
            m_FontManager.Remove(font.id);
        }
    }

    void SDLRenderer::DrawText(Font font, const std::string& text, Vector2 pos, Color color) {
        if (!m_Renderer)
            return;

        TTF_Font* f = m_FontManager.Get(font.id);
        if (!f)
            f = m_DefaultFont;

        SDL_Color    c    = {color.r, color.g, color.b, color.a};
        SDL_Surface* surf = TTF_RenderText_Solid(f, text.c_str(), c);
        if (!surf) {
            std::cerr << "Failed to render text: " << TTF_GetError() << std::endl;
            return;
        }
        SDL_Texture* tex = SDL_CreateTextureFromSurface(m_Renderer, surf);
        if (!tex) {
            std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surf);
            return;
        }
        SDL_FRect dst = {pos.x, pos.y, (float) surf->w, (float) surf->h};
        SDL_RenderCopyF(m_Renderer, tex, nullptr, &dst);
        SDL_DestroyTexture(tex);
        SDL_FreeSurface(surf);
    }

    void SDLRenderer::DrawText(const std::string& text, Vector2 pos, int fontSize, Color color) {
        if (!m_Renderer || !m_DefaultFont)
            return;

        TTF_SetFontSize(m_DefaultFont, fontSize);
        DrawText(Font{0}, text, pos, color);  // 0 = use m_DefaultFont
    }

}  // namespace ugfx::sdl
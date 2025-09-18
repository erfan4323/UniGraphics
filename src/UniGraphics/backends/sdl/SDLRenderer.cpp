#include "SDLRenderer.h"

#include <cmath>
#include <iostream>

#define M_PI 3.14159265358979323846  // pi

namespace ugfx::sdl {

    std::unordered_map<unsigned int, SDL_Texture*> SDLRenderer::g_textureMap;
    unsigned int                                   SDLRenderer::g_nextTextureId = 1;
    std::unordered_map<unsigned int, TTF_Font*>    SDLRenderer::g_fontMap;
    unsigned int                                   SDLRenderer::g_nextFontId = 1;

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
        m_DefaultFont = TTF_OpenFont("assets/default.ttf", 16);
        if (!m_DefaultFont) {
            std::cerr << "Failed to load default font: " << TTF_GetError() << std::endl;
        }
    }

    SDLRenderer::~SDLRenderer() {
        if (m_DefaultFont)
            TTF_CloseFont(m_DefaultFont);
        if (m_Renderer)
            SDL_DestroyRenderer(m_Renderer);
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
        } else {
            float dx     = end.x - start.x;
            float dy     = end.y - start.y;
            float length = sqrtf(dx * dx + dy * dy);
            if (length < 0.001f)
                return;
            float     angle = atan2f(dy, dx) * 180.0f / M_PI;
            SDL_FRect rect  = {start.x, start.y - thickness / 2, length, thickness};
            SDL_RenderFillRectF(m_Renderer, &rect);
        }
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
        SDL_FRect rect = {rec.x, rec.y, rec.width, rec.height};
        SDL_RenderDrawRectF(m_Renderer, &rect);
    }

    void SDLRenderer::DrawCircle(Vector2 center, float radius, Color color) {
        if (!m_Renderer)
            return;
        SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
        SDL_FRect rect = {center.x - radius, center.y - radius, radius * 2, radius * 2};
        SDL_RenderFillRectF(m_Renderer, &rect);  // Simplified circle (square for demo)
    }

    void SDLRenderer::DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color) {
        if (!m_Renderer)
            return;
        SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawLineF(m_Renderer, v1.x, v1.y, v2.x, v2.y);
        SDL_RenderDrawLineF(m_Renderer, v2.x, v2.y, v3.x, v3.y);
        SDL_RenderDrawLineF(m_Renderer, v3.x, v3.y, v1.x, v1.y);
    }

    Texture SDLRenderer::LoadTexture(const std::string& path) {
        if (!m_Renderer)
            return Texture{0, 0, 0};  // 0 means invalid

        SDL_Texture* tex = IMG_LoadTexture(m_Renderer, path.c_str());
        if (!tex) {
            std::cerr << "Failed to load texture: " << IMG_GetError() << std::endl;
            return Texture{0, 0, 0};
        }

        int w, h;
        SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);

        unsigned int id  = g_nextTextureId++;
        g_textureMap[id] = tex;

        return Texture{static_cast<int>(id), w, h};
    }

    void SDLRenderer::UnloadTexture(Texture tex) {
        auto it = g_textureMap.find(tex.id);
        if (it != g_textureMap.end()) {
            SDL_DestroyTexture(it->second);
            g_textureMap.erase(it);
        }
    }

    void SDLRenderer::DrawTexture(Texture tex, Vector2 pos, Color tint) {
        if (!m_Renderer || tex.id == 0)
            return;

        auto it = g_textureMap.find(tex.id);
        if (it == g_textureMap.end())
            return;

        SDL_Texture* realTex = it->second;

        SDL_SetTextureColorMod(realTex, tint.r, tint.g, tint.b);
        SDL_SetTextureAlphaMod(realTex, tint.a);

        SDL_FRect dst = {pos.x, pos.y, static_cast<float>(tex.width), static_cast<float>(tex.height)};
        SDL_RenderCopyF(m_Renderer, realTex, nullptr, &dst);
    }

    void SDLRenderer::DrawTextureRegion(Texture tex, Rectangle src, Vector2 dst, Color tint) {
        if (!m_Renderer || tex.id == 0)
            return;

        auto it = g_textureMap.find(tex.id);
        if (it == g_textureMap.end())
            return;

        SDL_Texture* realTex = it->second;

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

        auto it = g_textureMap.find(tex.id);
        if (it == g_textureMap.end())
            return;

        SDL_Texture* realTex = it->second;

        SDL_SetTextureColorMod(realTex, tint.r, tint.g, tint.b);
        SDL_SetTextureAlphaMod(realTex, tint.a);

        SDL_FRect  dst    = {pos.x, pos.y, tex.width * scale, tex.height * scale};
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
            return Font{0};  // invalid
        }

        unsigned int id = g_nextFontId++;
        g_fontMap[id]   = font;

        return Font{static_cast<int>(id)};
    }

    void SDLRenderer::UnloadFont(Font font) {
        auto it = g_fontMap.find(font.id);
        if (it != g_fontMap.end()) {
            TTF_CloseFont(it->second);
            g_fontMap.erase(it);
        }
    }

    void SDLRenderer::DrawText(Font font, const std::string& text, Vector2 pos, Color color) {
        if (!m_Renderer)
            return;

        TTF_Font* f = nullptr;
        if (font.id != 0) {
            auto it = g_fontMap.find(font.id);
            if (it != g_fontMap.end())
                f = it->second;
        } else {
            f = m_DefaultFont;  // fallback
        }
        if (!f)
            return;

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
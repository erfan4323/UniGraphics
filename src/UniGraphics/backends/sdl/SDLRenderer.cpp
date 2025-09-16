#include "SDLRenderer.h"

#include <cmath>
#include <iostream>

#define M_PI 3.14159265358979323846  // pi

namespace ugfx::sdl {

    SDLRenderer::SDLRenderer(SDL_Window* window) {
        m_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    }

    SDLRenderer::~SDLRenderer() {
        if (m_Renderer)
            SDL_DestroyRenderer(m_Renderer);
    }

    void SDLRenderer::BeginDrawing() {
    }

    void SDLRenderer::EndDrawing() {
        SDL_RenderPresent(m_Renderer);
    }

    void SDLRenderer::Clear(Color color) {
        SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
        SDL_RenderClear(m_Renderer);
    }

    void SDLRenderer::DrawPixel(Vector2 pos, Color color) {
        SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawPointF(m_Renderer, pos.x, pos.y);
    }

    void SDLRenderer::DrawLine(Vector2 start, Vector2 end, float thickness, Color color) {
        SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
        if (thickness <= 1.0f) {
            SDL_RenderDrawLineF(m_Renderer, start.x, start.y, end.x, end.y);
        } else {
            // Approximate thick line with a rectangle
            float dx     = end.x - start.x;
            float dy     = end.y - start.y;
            float length = sqrtf(dx * dx + dy * dy);
            if (length < 0.001f)
                return;
            float     angle = atan2f(dy, dx) * 180.0f / M_PI;
            SDL_FRect rect  = {start.x, start.y - thickness / 2, length, thickness};
            // SDL2 doesn't natively support rotated rectangles; use SDL2_gfx for better support
            SDL_RenderFillRectF(m_Renderer, &rect);  // Simplified, no rotation
        }
    }

    void SDLRenderer::DrawRectangle(Rectangle rec, Color color) {
        SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
        SDL_FRect rect = {rec.x, rec.y, rec.width, rec.height};
        SDL_RenderFillRectF(m_Renderer, &rect);
    }

    void SDLRenderer::DrawRectangleLines(Rectangle rec, float thickness, Color color) {
        SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
        SDL_FRect rect = {rec.x, rec.y, rec.width, rec.height};
        SDL_RenderDrawRectF(m_Renderer, &rect);  // SDL only supports 1-pixel lines
    }

    void SDLRenderer::DrawCircle(Vector2 center, float radius, Color color) {
        // Approximate circle with a rectangle (SDL2 lacks native circle support)
        SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
        SDL_FRect rect = {center.x - radius, center.y - radius, radius * 2, radius * 2};
        SDL_RenderFillRectF(m_Renderer, &rect);
    }

    void SDLRenderer::DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color) {
        // Approximate triangle by drawing lines (SDL2 lacks native triangle support)
        SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawLineF(m_Renderer, v1.x, v1.y, v2.x, v2.y);
        SDL_RenderDrawLineF(m_Renderer, v2.x, v2.y, v3.x, v3.y);
        SDL_RenderDrawLineF(m_Renderer, v3.x, v3.y, v1.x, v1.y);
    }

}  // namespace ugfx::sdl
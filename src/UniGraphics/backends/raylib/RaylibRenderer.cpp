#include "RaylibRenderer.h"

#include "RaylibConverter.h"

namespace ugfx::raylib {

    RaylibRenderer::RaylibRenderer() {
    }

    RaylibRenderer::~RaylibRenderer() {
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

}  // namespace ugfx::raylib
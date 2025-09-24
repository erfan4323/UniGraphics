#include <cmath>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "UniGraphics/UniGraphics.h"

using namespace ugfx;

// ---------------------------------------------------------------------
// Helper structs & functions (unchanged)
// ---------------------------------------------------------------------

struct BackendContext {
    std::unique_ptr<IGraphicsBackend> backend;
    IWindow*                          window   = nullptr;
    IInput*                           input    = nullptr;
    IRenderer*                        renderer = nullptr;
};

std::string GetBackendName(BackendType backend) {
    switch (backend) {
        case BackendType::SDL:
            return "SDL";
        case BackendType::Raylib:
            return "Raylib";
        case BackendType::OpenGL:
            return "OpenGL";
        case BackendType::Software:
            return "Software";
    }
    return "Unknown";
}

bool InitBackend(BackendContext& ctx, BackendType type, const std::string& title, int width, int height,
                 WindowFlags flags) {
    ctx.backend = CreateBackend(type);
    if (!ctx.backend) {
        std::cerr << "Failed to create backend\n";
        return false;
    }
    ctx.window   = ctx.backend->GetWindow();
    ctx.input    = ctx.backend->GetInput();
    ctx.renderer = ctx.backend->GetRenderer();

    if (!ctx.window || !ctx.input || !ctx.renderer) {
        std::cerr << "Failed to get backend interfaces\n";
        return false;
    }
    if (!ctx.window->Create(title, width, height, flags)) {
        std::cerr << "Failed to create window\n";
        return false;
    }
    ctx.window->SetTargetFPS(60);
    return true;
}

Texture LoadTextureSafe(IRenderer* renderer, const std::string& path) {
    Texture tex = renderer->LoadTexture(path);
    if (tex.id == -1)
        std::cerr << "Warning: Failed to load texture: " << path << std::endl;
    return tex;
}

Font LoadFontSafe(IRenderer* renderer, const std::string& path, int size) {
    Font font = renderer->LoadFont(path, size);
    if (font.id == -1)
        std::cerr << "Warning: Failed to load font: " << path << std::endl;
    return font;
}

// ---------------------------------------------------------------------
// Shared scene data (now holds a copy for each backend)
// ---------------------------------------------------------------------
struct Scene {
    Rectangle rect{0.0f, 0.0f, 50.0f, 50.0f};
    float     speed    = 250.0f;
    int       fontSize = 20;

    // SDL resources
    Texture sdlTexture;
    Font    sdlFont;

    // Raylib resources
    Texture rayTexture;
    Font    rayFont;
};

int main() {
    // -----------------------------------------------------------------
    // Configuration
    // -----------------------------------------------------------------
    const std::string windowTitle = "Renderer Demo - Dual Windows";
    const int         winW = 700, winH = 700;  // each window size
    WindowFlags       flags   = WindowFlags::Resizable;
    Color             bgColor = {30, 30, 60, 255};

    // -----------------------------------------------------------------
    // Create two backends: SDL (left) and Raylib (right)
    // -----------------------------------------------------------------
    BackendContext ctx_sdl, ctx_ray;

    if (!InitBackend(ctx_sdl, BackendType::SDL, windowTitle + " (SDL)", winW, winH, flags))
        return -1;
    if (!InitBackend(ctx_ray, BackendType::Raylib, windowTitle + " (Raylib)", winW, winH, flags))
        return -1;

    // -----------------------------------------------------------------
    // Load assets (once, shared)
    // -----------------------------------------------------------------
    std::string exeDir      = std::filesystem::current_path().string();
    std::string texturePath = exeDir + "/assets/BRICK_2B.png";
    std::string fontPath    = exeDir + "/assets/Lexend.ttf";

    // -----------------------------------------------------------------
    // Load assets (once per renderer)
    // -----------------------------------------------------------------
    Scene scene;

    // SDL side
    scene.sdlTexture = LoadTextureSafe(ctx_sdl.renderer, texturePath);
    scene.sdlFont    = LoadFontSafe(ctx_sdl.renderer, fontPath, scene.fontSize);

    // Raylib side (load the same files again, but through the Raylib renderer)
    scene.rayTexture = LoadTextureSafe(ctx_ray.renderer, texturePath);
    scene.rayFont    = LoadFontSafe(ctx_ray.renderer, fontPath, scene.fontSize);

    // -----------------------------------------------------------------
    // Main loop – update once, draw twice
    // -----------------------------------------------------------------
    float totalTime = 0.0f;

    while (!ctx_sdl.window->ShouldClose() && !ctx_ray.window->ShouldClose()) {
        // Poll events for both windows
        ctx_sdl.window->PollEvents();
        ctx_ray.window->PollEvents();

        // Use delta time from the first window (both run at the same FPS)
        float dt = ctx_sdl.window->GetDeltaTime();
        dt       = std::clamp(dt, 0.001f, 0.05f);
        totalTime += dt;

        // -------------------- Input (shared) --------------------
        Vector2 velocity{0, 0};

        // SDL side
        if (ctx_sdl.input->IsKeyDown(Key::right))
            velocity.x += scene.speed;
        if (ctx_sdl.input->IsKeyDown(Key::left))
            velocity.x -= scene.speed;
        if (ctx_sdl.input->IsKeyDown(Key::up))
            velocity.y -= scene.speed;
        if (ctx_sdl.input->IsKeyDown(Key::down))
            velocity.y += scene.speed;

        // Raylib side (in case it has focus)
        if (ctx_ray.input->IsKeyDown(Key::right))
            velocity.x += scene.speed;
        if (ctx_ray.input->IsKeyDown(Key::left))
            velocity.x -= scene.speed;
        if (ctx_ray.input->IsKeyDown(Key::up))
            velocity.y -= scene.speed;
        if (ctx_ray.input->IsKeyDown(Key::down))
            velocity.y += scene.speed;

        // Global quit
        if (ctx_sdl.input->IsKeyDown(Key::escape) || ctx_ray.input->IsKeyDown(Key::escape))
            break;

        // Font size changes
        if (ctx_sdl.input->IsKeyPressed(Key::f) || ctx_ray.input->IsKeyPressed(Key::f))
            scene.fontSize = std::min(scene.fontSize + 2, 40);
        if (ctx_sdl.input->IsKeyPressed(Key::g) || ctx_ray.input->IsKeyPressed(Key::g))
            scene.fontSize = std::max(scene.fontSize - 2, 10);

        // -------------------- Update scene --------------------
        scene.rect.x += velocity.x * dt;
        scene.rect.y += velocity.y * dt;

        // -----------------------------------------------------------------
        // Render helper – choose the correct assets for the renderer
        // -----------------------------------------------------------------
        auto RenderTo = [&](IRenderer* rend, const Vector2& viewOffset, bool useRaylib) {
            rend->BeginDrawing();
            rend->Clear(bgColor);

            // … background gradient unchanged …

            // Transform world → view
            Rectangle drawRect = scene.rect;
            drawRect.x -= viewOffset.x;
            drawRect.y -= viewOffset.y;

            // Outline
            rend->DrawRectangle(drawRect, {255, 255, 255, 255});

            // ----- Texture -----
            if (useRaylib) {
                if (scene.rayTexture.id != -1)
                    rend->DrawTexture(scene.rayTexture, {drawRect.x, drawRect.y}, {255, 255, 255, 255});
            } else {
                if (scene.sdlTexture.id != -1)
                    rend->DrawTexture(scene.sdlTexture, {drawRect.x, drawRect.y}, {255, 255, 255, 255});
            }

            // ----- Text -----
            if (useRaylib) {
                if (scene.rayFont.id != -1) {
                    std::string txt =
                        "Pos: (" + std::to_string((int) scene.rect.x) + ", " + std::to_string((int) scene.rect.y) + ")";
                    rend->DrawText(scene.sdlFont, txt, {drawRect.x, drawRect.y - 20}, {255, 255, 0, 255});
                }
            } else {
                if (scene.sdlFont.id != -1) {
                    std::string txt =
                        "Pos: (" + std::to_string((int) scene.rect.x) + ", " + std::to_string((int) scene.rect.y) + ")";
                    rend->DrawText(scene.rayFont, txt, {drawRect.x, drawRect.y - 20}, {255, 255, 0, 255});
                }
            }

            std::string txt = (useRaylib ? "Raylib" : "SDL") + std::string(" Backend");

            rend->DrawText(txt, {50, 50}, 20, {255, 255, 0, 255});

            rend->EndDrawing();
        };

        // -----------------------------------------------------------------
        // Render both windows (pass a flag so the lambda knows which assets to use)
        // -----------------------------------------------------------------
        RenderTo(ctx_sdl.renderer, {0.0f, 0.0f}, false);                     // SDL side
        RenderTo(ctx_ray.renderer, {static_cast<float>(winW), 0.0f}, true);  // Raylib side
    }

    // -----------------------------------------------------------------
    // Cleanup
    // -----------------------------------------------------------------
    if (scene.rayTexture.id != -1 && scene.sdlTexture.id != -1) {
        ctx_sdl.renderer->UnloadTexture(scene.sdlTexture);
        ctx_ray.renderer->UnloadTexture(scene.rayTexture);
    }
    if (scene.rayFont.id != -1 && scene.sdlFont.id != -1) {
        ctx_sdl.renderer->UnloadFont(scene.sdlFont);
        ctx_ray.renderer->UnloadFont(scene.rayFont);
    }

    return 0;
}

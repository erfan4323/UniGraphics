#include <cmath>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "UniGraphics/UniGraphics.h"

using namespace ugfx;

// ------------------- Helper Structs & Functions -------------------

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

// ------------------- Main Program -------------------

int main() {
    // Backends (exclude OpenGL and Software as not implemented)
    std::vector<BackendType> backends            = {BackendType::Raylib, BackendType::SDL};
    size_t                   currentBackendIndex = 0;

    // Window
    const std::string windowTitle = "Renderer Demo";
    const int         windowWidth = 1060, windowHeight = 630;
    const bool        fullscreen = false;

    // Demo objects
    Rectangle rect     = {0.0f, 0.0f, 50.0f, 50.0f};  // Relative to shapesPanel
    float     speed    = 250.0f;
    int       fontSize = 20;

    float totalTime = 0.0f;  // keeps track of absolute time
    float rotation  = 0.0f;

    // Asset paths
    std::string exeDir      = std::filesystem::current_path().string();
    std::string texturePath = exeDir + "/assets/BRICK_2B.png";
    std::string fontPath    = exeDir + "/assets/Lexend.ttf";

    std::cout << texturePath << '\n';

    WindowFlags flags = WindowFlags::Resizable;

    // Backend setup
    BackendContext ctx;
    if (!InitBackend(ctx, backends[currentBackendIndex], windowTitle, windowWidth, windowHeight, flags))
        return -1;

    // Load assets
    Texture texture = LoadTextureSafe(ctx.renderer, texturePath);
    Font    font    = LoadFontSafe(ctx.renderer, fontPath, fontSize);

    // Main loop
    while (!ctx.window->ShouldClose()) {
        ctx.window->PollEvents();
        float dt = ctx.window->GetDeltaTime();

        // Clamp dt to avoid huge jumps after backend switch
        dt = std::clamp(dt, 0.001f, 0.05f);

        totalTime += dt;
        rotation = 90.0f * totalTime;  // rotation in degrees

        // --- Input ---
        Vector2 velocity{0, 0};
        if (ctx.input->IsKeyDown(Key::right))
            velocity.x += speed;
        if (ctx.input->IsKeyDown(Key::left))
            velocity.x -= speed;
        if (ctx.input->IsKeyDown(Key::up))
            velocity.y -= speed;
        if (ctx.input->IsKeyDown(Key::down))
            velocity.y += speed;
        if (ctx.input->IsKeyDown(Key::escape))
            break;
        if (ctx.input->IsKeyPressed(Key::f))
            fontSize = std::min(fontSize + 2, 40);
        if (ctx.input->IsKeyPressed(Key::g))
            fontSize = std::max(fontSize - 2, 10);

        // --- Switch backend ---
        if (ctx.input->IsKeyPressed(Key::tab)) {
            currentBackendIndex = (currentBackendIndex + 1) % backends.size();

            // Cleanup assets
            if (texture.id != -1)
                ctx.renderer->UnloadTexture(texture);
            if (font.id != -1)
                ctx.renderer->UnloadFont(font);
            ctx.backend.reset();

            std::cout << "------------------------------------------------------------------------------\n";
            std::cout << "Switching to backend: " << GetBackendName(backends[currentBackendIndex]) << "\n";

            if (!InitBackend(ctx, backends[currentBackendIndex], windowTitle, windowWidth, windowHeight, flags))
                return -1;

            // Reload assets
            texture = LoadTextureSafe(ctx.renderer, texturePath);
            font    = LoadFontSafe(ctx.renderer, fontPath, fontSize);
        }

        // --- Update ---
        rect.x += velocity.x * dt;
        rect.y += velocity.y * dt;

        // Panels
        Rectangle shapesPanel   = {10, 10, 380, 300};
        Rectangle texturesPanel = {10, 320, 380, 300};
        Rectangle infoPanel     = {400, 10, 650, 610};

        // Clamp rectangle to shapesPanel
        rect.x = std::clamp(rect.x, 0.0f, (float) windowWidth);
        rect.y = std::clamp(rect.y, 0.0f, (float) windowHeight);

        // --- Render ---
        ctx.renderer->BeginDrawing();

        // Background gradient
        Color bgTop    = {30, 30, 60, 255};
        Color bgBottom = {80, 50, 120, 255};
        for (int y = 0; y < windowHeight; ++y) {
            float t = float(y) / windowHeight;
            Color c = {static_cast<unsigned char>(bgTop.r * (1 - t) + bgBottom.r * t),
                       static_cast<unsigned char>(bgTop.g * (1 - t) + bgBottom.g * t),
                       static_cast<unsigned char>(bgTop.b * (1 - t) + bgBottom.b * t), 255};
            ctx.renderer->DrawLine({0, float(y)}, {float(windowWidth), float(y)}, 1.0f, c);
        }

        // --- Panels / Frames ---
        ctx.renderer->DrawRectangleLines(shapesPanel, 3.0f, {255, 255, 255, 180});
        ctx.renderer->DrawRectangleLines(texturesPanel, 3.0f, {255, 255, 255, 180});
        ctx.renderer->DrawRectangleLines(infoPanel, 3.0f, {255, 255, 255, 180});

        // --- Shapes inside shapesPanel ---
        Vector2   shapesOffset = {shapesPanel.x + 10, shapesPanel.y + 10};
        Color     rectColor    = {static_cast<unsigned char>(128 + 127 * std::sin(totalTime * 2.0f)), 165, 0, 255};
        Rectangle rectLocal    = {rect.x + shapesOffset.x, rect.y + shapesOffset.y, rect.width, rect.height};
        ctx.renderer->DrawRectangle(rectLocal, rectColor);
        ctx.renderer->DrawPixel({rectLocal.x + 25, rectLocal.y + 25}, {0, 255, 255, 255});
        ctx.renderer->DrawLine({shapesOffset.x + 10, shapesOffset.y + 60}, {shapesOffset.x + 110, shapesOffset.y + 110},
                               2.0f, {0, 255, 100, 255});
        ctx.renderer->DrawRectangleLines({shapesOffset.x + 130, shapesOffset.y + 60, 80, 80}, 2.0f,
                                         {255, 220, 100, 255});
        ctx.renderer->DrawCircle({shapesOffset.x + 260, shapesOffset.y + 90}, 30.0f, {200, 100, 200, 255});
        ctx.renderer->DrawTriangle({shapesOffset.x + 260, shapesOffset.y + 150},
                                   {shapesOffset.x + 230, shapesOffset.y + 200},
                                   {shapesOffset.x + 290, shapesOffset.y + 200}, {255, 255, 255, 255});

        // --- Textures inside texturesPanel ---
        Vector2 texturesOffset = {texturesPanel.x + 50, texturesPanel.y + 50};
        if (texture.id != -1) {
            ctx.renderer->DrawTexture(texture, {texturesOffset.x, texturesOffset.y}, {255, 255, 255, 255});
            ctx.renderer->DrawTextureRegion(texture, {0, 0, 32, 32}, {texturesOffset.x + 80, texturesOffset.y},
                                            {255, 255, 255, 255});
            ctx.renderer->DrawTextureEx(texture, {texturesOffset.x + 160, texturesOffset.y + 32}, {32, 32}, rotation,
                                        1.2f, Flip::Horizontal, {200, 220, 255, 255});
        }

        // --- Info / help text inside infoPanel ---
        Vector2 infoOffset = {infoPanel.x + 10, infoPanel.y + 10};
        ctx.renderer->DrawText(font, "Renderer Demo", {infoOffset.x, infoOffset.y}, {255, 180, 80, 255});
        ctx.renderer->DrawText("Backend: " + GetBackendName(backends[currentBackendIndex]),
                               {infoOffset.x, infoOffset.y + 40}, fontSize, {150, 200, 255, 255});
        ctx.renderer->DrawText("Use Arrow Keys to Move Shapes", {infoOffset.x, infoOffset.y + 80}, fontSize,
                               {180, 255, 180, 255});
        ctx.renderer->DrawText("Press F/G to Change Font Size", {infoOffset.x, infoOffset.y + 120}, fontSize,
                               {180, 255, 180, 255});
        ctx.renderer->DrawText("Press TAB to Switch Backend", {infoOffset.x, infoOffset.y + 160}, fontSize,
                               {180, 255, 180, 255});
        ctx.renderer->DrawText("Press ESC to Exit", {infoOffset.x, infoOffset.y + 200}, fontSize, {255, 180, 180, 255});

        // Highlight around moving rectangle
        ctx.renderer->DrawRectangleLines({rectLocal.x - 2, rectLocal.y - 2, rectLocal.width + 4, rectLocal.height + 4},
                                         2.0f, {255, 255, 0, 200});

        ctx.renderer->EndDrawing();
    }

    // Cleanup
    if (texture.id != -1)
        ctx.renderer->UnloadTexture(texture);
    if (font.id != -1)
        ctx.renderer->UnloadFont(font);

    return 0;
}
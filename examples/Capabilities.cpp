#include <cmath>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "UniGraphics/UniGraphics.h"

// ------------------- Helper Structs & Functions -------------------

struct BackendContext {
    std::unique_ptr<ugfx::IGraphicsBackend> backend;
    ugfx::IWindow*                          window   = nullptr;
    ugfx::IInput*                           input    = nullptr;
    ugfx::IRenderer*                        renderer = nullptr;
};

bool InitBackend(BackendContext& ctx, const std::string& title, int width, int height, ugfx::WindowFlags flags) {
    ctx.backend = ugfx::CreateBackend();
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

ugfx::Texture LoadTextureSafe(ugfx::IRenderer* renderer, const std::string& path) {
    ugfx::Texture tex = renderer->LoadTexture(path);
    if (tex.id == -1)
        std::cerr << "Warning: Failed to load texture: " << path << std::endl;
    return tex;
}

ugfx::Font LoadFontSafe(ugfx::IRenderer* renderer, const std::string& path, int size) {
    ugfx::Font font = renderer->LoadFont(path, size);
    if (font.id == -1)
        std::cerr << "Warning: Failed to load font: " << path << std::endl;
    return font;
}

// ------------------- Main Program -------------------

int main() {
    // Backends (exclude OpenGL and Software as not implemented)
    size_t currentBackendIndex = 0;

    // Window
    const std::string windowTitle = "Renderer Demo";
    const int         windowWidth = 1060, windowHeight = 630;
    const bool        fullscreen = false;

    // Demo objects
    ugfx::Rectangle rect     = {0.0f, 0.0f, 50.0f, 50.0f};  // Relative to shapesPanel
    float           speed    = 250.0f;
    int             fontSize = 20;

    float totalTime = 0.0f;  // keeps track of absolute time
    float rotation  = 0.0f;

    // Asset paths
    std::string exeDir      = std::filesystem::current_path().string();
    std::string texturePath = exeDir + "/assets/BRICK_2B.png";
    std::string fontPath    = exeDir + "/assets/Lexend.ttf";

    std::cout << texturePath << '\n';

    ugfx::WindowFlags flags = ugfx::WindowFlags::Resizable;

    // Backend setup
    BackendContext ctx;
    if (!InitBackend(ctx, windowTitle, windowWidth, windowHeight, flags))
        return -1;

    // Load assets
    ugfx::Texture texture = LoadTextureSafe(ctx.renderer, texturePath);
    ugfx::Font    font    = LoadFontSafe(ctx.renderer, fontPath, fontSize);

    // Main loop
    while (!ctx.window->ShouldClose()) {
        ctx.window->PollEvents();
        float dt = ctx.window->GetDeltaTime();

        // Clamp dt to avoid huge jumps after backend switch
        dt = std::clamp(dt, 0.001f, 0.05f);

        totalTime += dt;
        rotation = 90.0f * totalTime;  // rotation in degrees

        // --- Input ---
        ugfx::Vector2 velocity{0, 0};
        if (ctx.input->IsKeyDown(ugfx::Key::right))
            velocity.x += speed;
        if (ctx.input->IsKeyDown(ugfx::Key::left))
            velocity.x -= speed;
        if (ctx.input->IsKeyDown(ugfx::Key::up))
            velocity.y -= speed;
        if (ctx.input->IsKeyDown(ugfx::Key::down))
            velocity.y += speed;
        if (ctx.input->IsKeyDown(ugfx::Key::escape))
            break;
        if (ctx.input->IsKeyPressed(ugfx::Key::f))
            fontSize = std::min(fontSize + 2, 40);
        if (ctx.input->IsKeyPressed(ugfx::Key::g))
            fontSize = std::max(fontSize - 2, 10);

        // --- Update ---
        rect.x += velocity.x * dt;
        rect.y += velocity.y * dt;

        // Panels
        ugfx::Rectangle shapesPanel   = {10, 10, 380, 300};
        ugfx::Rectangle texturesPanel = {10, 320, 380, 300};
        ugfx::Rectangle infoPanel     = {400, 10, 650, 610};

        // Clamp rectangle to shapesPanel
        rect.x = std::clamp(rect.x, 0.0f, (float) windowWidth);
        rect.y = std::clamp(rect.y, 0.0f, (float) windowHeight);

        // --- Render ---
        ctx.renderer->BeginDrawing();

        // Background gradient
        ugfx::Color bgTop    = {30, 30, 60, 255};
        ugfx::Color bgBottom = {80, 50, 120, 255};
        for (int y = 0; y < windowHeight; ++y) {
            float       t = float(y) / windowHeight;
            ugfx::Color c = {static_cast<unsigned char>(bgTop.r * (1 - t) + bgBottom.r * t),
                             static_cast<unsigned char>(bgTop.g * (1 - t) + bgBottom.g * t),
                             static_cast<unsigned char>(bgTop.b * (1 - t) + bgBottom.b * t), 255};
            ctx.renderer->DrawLine({0, float(y)}, {float(windowWidth), float(y)}, 1.0f, c);
        }

        // --- Panels / Frames ---
        ctx.renderer->DrawRectangleLines(shapesPanel, 3.0f, {255, 255, 255, 180});
        ctx.renderer->DrawRectangleLines(texturesPanel, 3.0f, {255, 255, 255, 180});
        ctx.renderer->DrawRectangleLines(infoPanel, 3.0f, {255, 255, 255, 180});

        // --- Shapes inside shapesPanel ---
        ugfx::Vector2   shapesOffset = {shapesPanel.x + 10, shapesPanel.y + 10};
        ugfx::Color     rectColor = {static_cast<unsigned char>(128 + 127 * std::sin(totalTime * 2.0f)), 165, 0, 255};
        ugfx::Rectangle rectLocal = {rect.x + shapesOffset.x, rect.y + shapesOffset.y, rect.width, rect.height};
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
        ugfx::Vector2 texturesOffset = {texturesPanel.x + 50, texturesPanel.y + 50};
        if (texture.id != -1) {
            ctx.renderer->DrawTexture(texture, {texturesOffset.x, texturesOffset.y}, {255, 255, 255, 255});
            ctx.renderer->DrawTextureRegion(texture, {0, 0, 32, 32}, {texturesOffset.x + 80, texturesOffset.y},
                                            {255, 255, 255, 255});
            ctx.renderer->DrawTextureEx(texture, {texturesOffset.x + 160, texturesOffset.y + 32}, {32, 32}, rotation,
                                        1.2f, ugfx::Flip::Horizontal, {200, 220, 255, 255});
        }

        // --- Info / help text inside infoPanel ---
        ugfx::Vector2 infoOffset = {infoPanel.x + 10, infoPanel.y + 10};
        ctx.renderer->DrawText(font, "Renderer Demo", {infoOffset.x, infoOffset.y}, {255, 180, 80, 255});
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

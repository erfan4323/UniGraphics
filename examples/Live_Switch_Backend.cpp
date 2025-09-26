#include <filesystem>
#include <iostream>
#include <vector>

#include "UniGraphics/UniGraphics.h"

using namespace ugfx;

struct BackendContext {
    std::unique_ptr<IGraphicsBackend> backend;
    IWindow*                          window   = nullptr;
    IInput*                           input    = nullptr;
    IRenderer*                        renderer = nullptr;
};

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

int main() {
    std::vector<BackendType> backendTypes        = {BackendType::SDL, BackendType::Raylib};
    size_t                   currentBackendIndex = 0;

    // Window settings
    std::string windowTitle  = "Movable Rectangle Demo";
    int         windowWidth  = 800;
    int         windowHeight = 600;
    WindowFlags flags        = WindowFlags::AlwaysOnTop;

    // Rectangle
    Rectangle rect      = {100.0f, 100.0f, 50.0f, 50.0f};
    float     speed     = 250.0f;
    Color     rectColor = {255, 0, 0, 255};
    Color     bgColor   = {21, 21, 21, 255};

    BackendContext ctx;
    if (!InitBackend(ctx, backendTypes[currentBackendIndex], windowTitle, windowWidth, windowHeight, flags)) {
        return -1;
    }

    // Asset paths
    std::string exeDir   = std::filesystem::current_path().string();
    std::string fontPath = exeDir + "/assets/Lexend.ttf";
    Font        font     = ctx.renderer->LoadFont(fontPath, 20);

    // Main loop
    while (!ctx.window->ShouldClose()) {
        ctx.window->PollEvents();
        float deltaTime = ctx.window->GetDeltaTime();

        // Input
        Vector2 velocity = {0.0f, 0.0f};
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

        // Switch backend
        if (ctx.input->IsKeyPressed(Key::tab)) {
            ctx.backend.reset();  // cleanup
            currentBackendIndex = (currentBackendIndex + 1) % backendTypes.size();

            std::cout << "--------------------------------------------------------------------------------\n";
            std::cout << "Switching to backend: "
                      << (backendTypes[currentBackendIndex] == BackendType::SDL ? "SDL" : "Raylib") << '\n';

            if (!InitBackend(ctx, backendTypes[currentBackendIndex], windowTitle, windowWidth, windowHeight, flags)) {
                return -1;
            }
        }

        // Update rectangle
        rect.x += velocity.x * deltaTime;
        rect.y += velocity.y * deltaTime;

        // Clamp
        auto [winWidth, winHeight] = ctx.window->GetSize();
        rect.x                     = std::max(0.0f, std::min(rect.x, static_cast<float>(winWidth) - rect.width));
        rect.y                     = std::max(0.0f, std::min(rect.y, static_cast<float>(winHeight) - rect.height));

        // Render
        ctx.renderer->BeginDrawing();
        ctx.renderer->Clear(bgColor);
        ctx.renderer->DrawRectangle(rect, rectColor);

        std::string txt =
            (backendTypes[currentBackendIndex] == BackendType::Raylib ? "Raylib" : "SDL") + std::string(" Backend");
        ctx.renderer->DrawText(txt, {50, 50}, 20, {255, 255, 0, 255});

        ctx.renderer->EndDrawing();
    }

    return 0;
}

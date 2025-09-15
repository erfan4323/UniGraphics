#include "UniGraphics/UniGraphics.h"

using namespace ugfx;

int main() {
    // Create the Raylib backend
    auto backend = CreateBackend(BackendType::Raylib);
    if (!backend) {
        return -1;
    }

    // Get interfaces
    auto window   = backend->GetWindow();
    auto input    = backend->GetInput();
    auto renderer = backend->GetRenderer();

    // Create window
    if (!window->Create("Movable Rectangle Demo", 800, 600, false)) {
        return -1;
    }

    // Rectangle properties
    Rectangle rect      = {100.0f, 100.0f, 50.0f, 50.0f};  // x, y, width, height
    float     speed     = 100.0f;                          // Pixels per second
    Color     rectColor = {255, 0, 0, 255};                // Red
    Color     bgColor   = {0, 0, 0, 255};                  // Black background

    // Main loop
    float deltaTime = 1.0f / 60.0f;  // Assume 60 FPS for simplicity
    while (!window->ShouldClose()) {
        window->PollEvents();

        // Handle input
        Vector2 velocity = {0.0f, 0.0f};
        if (input->IsKeyDown(Key::RIGHT))
            velocity.x += speed;
        if (input->IsKeyDown(Key::LEFT))
            velocity.x -= speed;
        if (input->IsKeyDown(Key::UP))
            velocity.y -= speed;
        if (input->IsKeyDown(Key::DOWN))
            velocity.y += speed;
        if (input->IsKeyDown(Key::ESCAPE))
            break;

        // Update rectangle position
        rect.x += velocity.x * deltaTime;
        rect.y += velocity.y * deltaTime;

        // Clamp to window bounds
        auto [winWidth, winHeight] = window->GetSize();
        if (rect.x < 0)
            rect.x = 0;
        if (rect.y < 0)
            rect.y = 0;
        if (rect.x + rect.width > winWidth)
            rect.x = static_cast<float>(winWidth) - rect.width;
        if (rect.y + rect.height > winHeight)
            rect.y = static_cast<float>(winHeight) - rect.height;

        // Render
        renderer->BeginDrawing();
        renderer->Clear(bgColor);
        renderer->DrawRectangle(rect, rectColor);
        renderer->EndDrawing();
    }

    return 0;
}
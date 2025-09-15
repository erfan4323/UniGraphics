#include "RaylibWindow.h"

namespace ugfx::raylib {

    RaylibWindow::RaylibWindow() {
    }

    RaylibWindow::~RaylibWindow() {
        Shutdown();
    }

    bool RaylibWindow::Create(const std::string& title, int width, int height, bool fullscreen) {
        InitWindow(width, height, title.c_str());
        if (fullscreen) {
            ToggleFullscreen();
        }
        return IsWindowReady();
    }

    void RaylibWindow::SetTitle(const std::string& title) {
        SetWindowTitle(title.c_str());
    }

    std::pair<int, int> RaylibWindow::GetSize() const {
        return {GetScreenWidth(), GetScreenHeight()};
    }

    bool RaylibWindow::ShouldClose() const {
        return WindowShouldClose();
    }

    void RaylibWindow::PollEvents() {
        // Raylib handles events internally; no-op unless custom events needed
    }

    void RaylibWindow::Shutdown() {
        if (IsWindowReady()) {
            CloseWindow();
        }
    }

}  // namespace ugfx::raylib
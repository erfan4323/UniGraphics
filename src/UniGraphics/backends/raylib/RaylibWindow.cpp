#include "RaylibWindow.h"

namespace ugfx::raylib {

    RaylibWindow::RaylibWindow() {
    }

    RaylibWindow::~RaylibWindow() {
        Shutdown();
    }

    bool RaylibWindow::Create(const std::string& title, int width, int height, WindowFlags flags) {
        unsigned int rlFlags = 0;

        if (HasFlag(flags, WindowFlags::Fullscreen))
            rlFlags |= FLAG_FULLSCREEN_MODE;
        if (HasFlag(flags, WindowFlags::Borderless))
            rlFlags |= FLAG_WINDOW_UNDECORATED;
        if (HasFlag(flags, WindowFlags::Resizable))
            rlFlags |= FLAG_WINDOW_RESIZABLE;
        if (HasFlag(flags, WindowFlags::VSync))
            rlFlags |= FLAG_VSYNC_HINT;
        if (HasFlag(flags, WindowFlags::Hidden))
            rlFlags |= FLAG_WINDOW_HIDDEN;
        if (HasFlag(flags, WindowFlags::AlwaysOnTop))
            rlFlags |= FLAG_WINDOW_TOPMOST;

        SetConfigFlags(rlFlags);
        InitWindow(width, height, title.c_str());
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

    void RaylibWindow::SetTargetFPS(int fps) {
        ::SetTargetFPS(fps);
    }

    float RaylibWindow::GetDeltaTime() const {
        return GetFrameTime();
    }

    uint32_t RaylibWindow::GetTicks() const {
        return static_cast<uint32_t>(GetTime() * 1000.0);
    }

}  // namespace ugfx::raylib
#pragma once

#include <raylib.h>

#include "UniGraphics.h"

namespace ugfx::raylib {

    class RaylibWindow : public IWindow {
       public:
        RaylibWindow();
        ~RaylibWindow() override;

        bool                Create(const std::string& title, int width, int height, WindowFlags flags) override;
        void                SetTitle(const std::string& title) override;
        std::pair<int, int> GetSize() const override;
        bool                ShouldClose() const override;
        void                PollEvents() override;
        void                Shutdown() override;
        void                SetTargetFPS(int fps) override;
        float               GetDeltaTime() const override;
        uint32_t            GetTicks() const override;
        void*               GetHandle() const override;
    };

}  // namespace ugfx::raylib
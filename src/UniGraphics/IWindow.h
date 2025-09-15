#pragma once

#include <string>
#include <utility>

#include "CommonTypes.h"

namespace ugfx {

    class IWindow {
       public:
        virtual ~IWindow() = default;

        virtual bool                Create(const std::string& title, int width, int height, bool fullscreen) = 0;
        virtual void                SetTitle(const std::string& title)                                       = 0;
        virtual std::pair<int, int> GetSize() const                                                          = 0;
        virtual bool                ShouldClose() const                                                      = 0;
        virtual void                PollEvents()                                                             = 0;
        virtual void                Shutdown()                                                               = 0;
        virtual void                SetTargetFPS(int fps)                                                    = 0;
        virtual float               GetDeltaTime() const                                                     = 0;
    };

}  // namespace ugfx
#pragma once

#include <string>
#include <utility>

#include "CommonTypes.h"

namespace ugfx {

    enum class WindowFlags : uint32_t {
        None        = 0,
        Fullscreen  = 1 << 0,
        Borderless  = 1 << 1,
        Resizable   = 1 << 2,
        VSync       = 1 << 3,
        Hidden      = 1 << 4,
        AlwaysOnTop = 1 << 5
        // Add more as needed
    };

    class IWindow {
       public:
        virtual ~IWindow() = default;

        virtual bool                Create(const std::string& title, int width, int height, WindowFlags flags) = 0;
        virtual void                SetTitle(const std::string& title)                                         = 0;
        virtual std::pair<int, int> GetSize() const                                                            = 0;
        virtual bool                ShouldClose() const                                                        = 0;
        virtual void                PollEvents()                                                               = 0;
        virtual void                Shutdown()                                                                 = 0;
        virtual void                SetTargetFPS(int fps)                                                      = 0;
        virtual float               GetDeltaTime() const                                                       = 0;
        virtual uint32_t            GetTicks() const                                                           = 0;
        virtual void*               GetHandle() const                                                          = 0;
    };

    inline WindowFlags operator|(WindowFlags a, WindowFlags b) {
        return static_cast<WindowFlags>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
    }

    inline WindowFlags& operator|=(WindowFlags& a, WindowFlags b) {
        a = a | b;
        return a;
    }

    inline bool HasFlag(WindowFlags flags, WindowFlags test) {
        return (static_cast<uint32_t>(flags) & static_cast<uint32_t>(test)) != 0;
    }

}  // namespace ugfx
#pragma once

#include <functional>

#include "../CommonTypes.h"

namespace ugfx {

    class IInput {
       public:
        virtual ~IInput() = default;

        using EventCallback = std::function<void(void* event)>;

        virtual void ProcessEvents(void* event)                    = 0;
        virtual void BeginFrame()                                  = 0;
        virtual void RegisterEventCallback(EventCallback callback) = 0;

        virtual bool  IsKeyPressed(Key key) const  = 0;
        virtual bool  IsKeyDown(Key key) const     = 0;
        virtual bool  IsKeyReleased(Key key) const = 0;
        virtual bool  IsKeyUp(Key key) const       = 0;
        virtual void* GetHandle() const            = 0;
    };

}  // namespace ugfx

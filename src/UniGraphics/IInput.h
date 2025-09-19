#pragma once

#include "CommonTypes.h"

namespace ugfx {

    class IInput {
       public:
        virtual ~IInput() = default;

        virtual void ProcessEvents(void* event) = 0;

        virtual bool IsKeyPressed(Key key) const  = 0;
        virtual bool IsKeyDown(Key key) const     = 0;
        virtual bool IsKeyReleased(Key key) const = 0;
        virtual bool IsKeyUp(Key key) const       = 0;
    };

}  // namespace ugfx
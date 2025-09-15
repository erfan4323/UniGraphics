#pragma once

#include "CommonTypes.h"

class IInput {
   public:
    virtual ~IInput() = default;

    virtual bool IsKeyPressed(Key key) const  = 0;
    virtual bool IsKeyDown(Key key) const     = 0;
    virtual bool IsKeyReleased(Key key) const = 0;
    virtual bool IsKeyUp(Key key) const       = 0;
};
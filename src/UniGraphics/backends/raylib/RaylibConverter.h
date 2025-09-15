#pragma once

#include "CommonTypes.h"
#include "raylib.h"

namespace ugfx::raylib {

    // Convert to raylib types
    inline ::Vector2 ToRaylib(const Vector2& v) {
        return {v.x, v.y};
    }

    inline ::Vector3 ToRaylib(const Vector3& v) {
        return {v.x, v.y, v.z};
    }

    inline ::Color ToRaylib(const Color& c) {
        return {c.r, c.g, c.b, c.a};
    }

    inline ::Rectangle ToRaylib(const Rectangle& r) {
        return {(float) r.x, (float) r.y, (float) r.width, (float) r.height};
    }

    // Convert from raylib types
    inline Vector2 FromRaylib(const ::Vector2& v) {
        return {v.x, v.y};
    }

    inline Color FromRaylib(const ::Color& c) {
        return {c.r, c.g, c.b, c.a};
    }

    inline Rectangle FromRaylib(const ::Rectangle& r) {
        return {r.x, r.y, r.width, r.height};
    }

}  // namespace ugfx::raylib

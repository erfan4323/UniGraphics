#pragma once

namespace ugfx {

    struct Vector2 {
        float x;
        float y;
    };

    struct Vector3 {
        float x;
        float y;
        float z;
    };

    struct Vector4 {
        float x;
        float y;
        float z;
        float w;
    };

    struct Color {
        unsigned char r, g, b, a;
    };

    struct Rectangle {
        float x, y, width, height;
    };

    struct Texture {
        int id     = -1;  // Invalid by default
        int width  = 0;
        int height = 0;
    };

    struct Font {
        int id = -1;  // Invalid by default
    };

    enum class Flip { None, Horizontal, Vertical };

    enum class Key {
        key_null = 0,  // Key: NULL, used for no key pressed
        // Alphanumeric keys
        apostrophe    = 39,  // Key: '
        comma         = 44,  // Key: ,
        minus         = 45,  // Key: -
        period        = 46,  // Key: .
        slash         = 47,  // Key: /
        zero          = 48,  // Key: 0
        one           = 49,  // Key: 1
        two           = 50,  // Key: 2
        three         = 51,  // Key: 3
        four          = 52,  // Key: 4
        five          = 53,  // Key: 5
        six           = 54,  // Key: 6
        seven         = 55,  // Key: 7
        eight         = 56,  // Key: 8
        nine          = 57,  // Key: 9
        semicolon     = 59,  // Key: ;
        equal         = 61,  // Key: =
        a             = 65,  // Key: A | a
        b             = 66,  // Key: B | b
        c             = 67,  // Key: C | c
        d             = 68,  // Key: D | d
        e             = 69,  // Key: E | e
        f             = 70,  // Key: F | f
        g             = 71,  // Key: G | g
        h             = 72,  // Key: H | h
        i             = 73,  // Key: I | i
        j             = 74,  // Key: J | j
        k             = 75,  // Key: K | k
        l             = 76,  // Key: L | l
        m             = 77,  // Key: M | m
        n             = 78,  // Key: N | n
        o             = 79,  // Key: O | o
        p             = 80,  // Key: P | p
        q             = 81,  // Key: Q | q
        r             = 82,  // Key: R | r
        s             = 83,  // Key: S | s
        t             = 84,  // Key: T | t
        u             = 85,  // Key: U | u
        v             = 86,  // Key: V | v
        w             = 87,  // Key: W | w
        x             = 88,  // Key: X | x
        y             = 89,  // Key: Y | y
        z             = 90,  // Key: Z | z
        left_bracket  = 91,  // Key: [
        backslash     = 92,  // Key: '\'
        right_bracket = 93,  // Key: ]
        grave         = 96,  // Key: `
        // Function keys
        space         = 32,   // Key: Space
        escape        = 256,  // Key: Esc
        enter         = 257,  // Key: Enter
        tab           = 258,  // Key: Tab
        backspace     = 259,  // Key: Backspace
        insert        = 260,  // Key: Ins
        delete_       = 261,  // Key: Del
        right         = 262,  // Key: Cursor right
        left          = 263,  // Key: Cursor left
        down          = 264,  // Key: Cursor down
        up            = 265,  // Key: Cursor up
        page_up       = 266,  // Key: Page up
        page_down     = 267,  // Key: Page down
        home          = 268,  // Key: Home
        end           = 269,  // Key: End
        caps_lock     = 280,  // Key: Caps lock
        scroll_lock   = 281,  // Key: Scroll down
        num_lock      = 282,  // Key: Num lock
        print_screen  = 283,  // Key: Print screen
        pause         = 284,  // Key: Pause
        f1            = 290,  // Key: F1
        f2            = 291,  // Key: F2
        f3            = 292,  // Key: F3
        f4            = 293,  // Key: F4
        f5            = 294,  // Key: F5
        f6            = 295,  // Key: F6
        f7            = 296,  // Key: F7
        f8            = 297,  // Key: F8
        f9            = 298,  // Key: F9
        f10           = 299,  // Key: F10
        f11           = 300,  // Key: F11
        f12           = 301,  // Key: F12
        left_shift    = 340,  // Key: Shift left
        left_control  = 341,  // Key: Control left
        left_alt      = 342,  // Key: Alt left
        left_super    = 343,  // Key: Super left
        right_shift   = 344,  // Key: Shift right
        right_control = 345,  // Key: Control right
        right_alt     = 346,  // Key: Alt right
        right_super   = 347,  // Key: Super right
        kb_menu       = 348,  // Key: KB menu
        // Keypad keys
        kp_0        = 320,  // Key: Keypad 0
        kp_1        = 321,  // Key: Keypad 1
        kp_2        = 322,  // Key: Keypad 2
        kp_3        = 323,  // Key: Keypad 3
        kp_4        = 324,  // Key: Keypad 4
        kp_5        = 325,  // Key: Keypad 5
        kp_6        = 326,  // Key: Keypad 6
        kp_7        = 327,  // Key: Keypad 7
        kp_8        = 328,  // Key: Keypad 8
        kp_9        = 329,  // Key: Keypad 9
        kp_decimal  = 330,  // Key: Keypad .
        kp_divide   = 331,  // Key: Keypad /
        kp_multiply = 332,  // Key: Keypad *
        kp_subtract = 333,  // Key: Keypad -
        kp_add      = 334,  // Key: Keypad +
        kp_enter    = 335,  // Key: Keypad Enter
        kp_equal    = 336,  // Key: Keypad =
        // Android key buttons
        back        = 4,   // Key: Android back button
        menu        = 82,  // Key: Android menu button
        volume_up   = 24,  // Key: Android volume up button
        volume_down = 25   // Key: Android volume down button
    };

    enum class BackendType { SDL, Raylib, OpenGL, Software };

}  // namespace ugfx
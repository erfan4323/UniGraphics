#include "SDLInput.h"

#include <SDL2/SDL.h>

#include <algorithm>

namespace ugfx::sdl {

    SDLInput::SDLInput() {
        m_CurrentDown.reset();
        m_PressedThisFrame.reset();
        m_ReleasedThisFrame.reset();
    }

    SDLInput::~SDLInput() = default;

    void* SDLInput::GetHandle() const {
        return static_cast<void*>(const_cast<SDL_Event*>(&m_LastEvent));
    }

    void SDLInput::ProcessEvents(void* event) {
        // m_CurrentDown.reset();
        m_PressedThisFrame.reset();
        m_ReleasedThisFrame.reset();

        auto sdlEvent = static_cast<SDL_Event*>(event);
        m_LastEvent   = *sdlEvent;

        switch (sdlEvent->type) {
            case SDL_KEYDOWN: {
                if (!sdlEvent->key.repeat) {
                    SDL_Scancode sc = sdlEvent->key.keysym.scancode;
                    if (!m_CurrentDown.test(sc)) {
                        m_CurrentDown.set(sc);
                        m_PressedThisFrame.set(sc);
                    }
                }
            } break;
            case SDL_KEYUP: {
                SDL_Scancode sc = sdlEvent->key.keysym.scancode;
                m_CurrentDown.reset(sc);
                m_ReleasedThisFrame.set(sc);
            } break;
            default:
                break;
        }
    }

    bool SDLInput::IsKeyDown(Key key) const {
        return m_CurrentDown.test(MapKey(key));
    }

    bool SDLInput::IsKeyPressed(Key key) const {
        return m_PressedThisFrame.test(MapKey(key));
    }

    bool SDLInput::IsKeyReleased(Key key) const {
        return m_ReleasedThisFrame.test(MapKey(key));
    }

    bool SDLInput::IsKeyUp(Key key) const {
        return !m_CurrentDown.test(MapKey(key));
    }

    SDL_Scancode SDLInput::MapKey(Key key) const {
        switch (key) {
            case Key::key_null:
                return SDL_SCANCODE_UNKNOWN;
            case Key::apostrophe:
                return SDL_SCANCODE_APOSTROPHE;
            case Key::comma:
                return SDL_SCANCODE_COMMA;
            case Key::minus:
                return SDL_SCANCODE_MINUS;
            case Key::period:
                return SDL_SCANCODE_PERIOD;
            case Key::slash:
                return SDL_SCANCODE_SLASH;
            case Key::zero:
                return SDL_SCANCODE_0;
            case Key::one:
                return SDL_SCANCODE_1;
            case Key::two:
                return SDL_SCANCODE_2;
            case Key::three:
                return SDL_SCANCODE_3;
            case Key::four:
                return SDL_SCANCODE_4;
            case Key::five:
                return SDL_SCANCODE_5;
            case Key::six:
                return SDL_SCANCODE_6;
            case Key::seven:
                return SDL_SCANCODE_7;
            case Key::eight:
                return SDL_SCANCODE_8;
            case Key::nine:
                return SDL_SCANCODE_9;
            case Key::semicolon:
                return SDL_SCANCODE_SEMICOLON;
            case Key::equal:
                return SDL_SCANCODE_EQUALS;
            case Key::a:
                return SDL_SCANCODE_A;
            case Key::b:
                return SDL_SCANCODE_B;
            case Key::c:
                return SDL_SCANCODE_C;
            case Key::d:
                return SDL_SCANCODE_D;
            case Key::e:
                return SDL_SCANCODE_E;
            case Key::f:
                return SDL_SCANCODE_F;
            case Key::g:
                return SDL_SCANCODE_G;
            case Key::h:
                return SDL_SCANCODE_H;
            case Key::i:
                return SDL_SCANCODE_I;
            case Key::j:
                return SDL_SCANCODE_J;
            case Key::k:
                return SDL_SCANCODE_K;
            case Key::l:
                return SDL_SCANCODE_L;
            case Key::m:
                return SDL_SCANCODE_M;
            case Key::n:
                return SDL_SCANCODE_N;
            case Key::o:
                return SDL_SCANCODE_O;
            case Key::p:
                return SDL_SCANCODE_P;
            case Key::q:
                return SDL_SCANCODE_Q;
            case Key::r:
                return SDL_SCANCODE_R;
            case Key::s:
                return SDL_SCANCODE_S;
            case Key::t:
                return SDL_SCANCODE_T;
            case Key::u:
                return SDL_SCANCODE_U;
            case Key::v:
                return SDL_SCANCODE_V;
            case Key::w:
                return SDL_SCANCODE_W;
            case Key::x:
                return SDL_SCANCODE_X;
            case Key::y:
                return SDL_SCANCODE_Y;
            case Key::z:
                return SDL_SCANCODE_Z;
            case Key::left_bracket:
                return SDL_SCANCODE_LEFTBRACKET;
            case Key::backslash:
                return SDL_SCANCODE_BACKSLASH;
            case Key::right_bracket:
                return SDL_SCANCODE_RIGHTBRACKET;
            case Key::grave:
                return SDL_SCANCODE_GRAVE;
            case Key::space:
                return SDL_SCANCODE_SPACE;
            case Key::escape:
                return SDL_SCANCODE_ESCAPE;
            case Key::enter:
                return SDL_SCANCODE_RETURN;
            case Key::tab:
                return SDL_SCANCODE_TAB;
            case Key::backspace:
                return SDL_SCANCODE_BACKSPACE;
            case Key::insert:
                return SDL_SCANCODE_INSERT;
            case Key::delete_:
                return SDL_SCANCODE_DELETE;
            case Key::right:
                return SDL_SCANCODE_RIGHT;
            case Key::left:
                return SDL_SCANCODE_LEFT;
            case Key::down:
                return SDL_SCANCODE_DOWN;
            case Key::up:
                return SDL_SCANCODE_UP;
            case Key::page_up:
                return SDL_SCANCODE_PAGEUP;
            case Key::page_down:
                return SDL_SCANCODE_PAGEDOWN;
            case Key::home:
                return SDL_SCANCODE_HOME;
            case Key::end:
                return SDL_SCANCODE_END;
            case Key::caps_lock:
                return SDL_SCANCODE_CAPSLOCK;
            case Key::scroll_lock:
                return SDL_SCANCODE_SCROLLLOCK;
            case Key::num_lock:
                return SDL_SCANCODE_NUMLOCKCLEAR;
            case Key::print_screen:
                return SDL_SCANCODE_PRINTSCREEN;
            case Key::pause:
                return SDL_SCANCODE_PAUSE;
            case Key::f1:
                return SDL_SCANCODE_F1;
            case Key::f2:
                return SDL_SCANCODE_F2;
            case Key::f3:
                return SDL_SCANCODE_F3;
            case Key::f4:
                return SDL_SCANCODE_F4;
            case Key::f5:
                return SDL_SCANCODE_F5;
            case Key::f6:
                return SDL_SCANCODE_F6;
            case Key::f7:
                return SDL_SCANCODE_F7;
            case Key::f8:
                return SDL_SCANCODE_F8;
            case Key::f9:
                return SDL_SCANCODE_F9;
            case Key::f10:
                return SDL_SCANCODE_F10;
            case Key::f11:
                return SDL_SCANCODE_F11;
            case Key::f12:
                return SDL_SCANCODE_F12;
            case Key::left_shift:
                return SDL_SCANCODE_LSHIFT;
            case Key::left_control:
                return SDL_SCANCODE_LCTRL;
            case Key::left_alt:
                return SDL_SCANCODE_LALT;
            case Key::left_super:
                return SDL_SCANCODE_LGUI;
            case Key::right_shift:
                return SDL_SCANCODE_RSHIFT;
            case Key::right_control:
                return SDL_SCANCODE_RCTRL;
            case Key::right_alt:
                return SDL_SCANCODE_RALT;
            case Key::right_super:
                return SDL_SCANCODE_RGUI;

            case Key::kp_0:
                return SDL_SCANCODE_KP_0;
            case Key::kp_1:
                return SDL_SCANCODE_KP_1;
            case Key::kp_2:
                return SDL_SCANCODE_KP_2;
            case Key::kp_3:
                return SDL_SCANCODE_KP_3;
            case Key::kp_4:
                return SDL_SCANCODE_KP_4;
            case Key::kp_5:
                return SDL_SCANCODE_KP_5;
            case Key::kp_6:
                return SDL_SCANCODE_KP_6;
            case Key::kp_7:
                return SDL_SCANCODE_KP_7;
            case Key::kp_8:
                return SDL_SCANCODE_KP_8;
            case Key::kp_9:
                return SDL_SCANCODE_KP_9;
            case Key::kp_decimal:
                return SDL_SCANCODE_KP_PERIOD;
            case Key::kp_divide:
                return SDL_SCANCODE_KP_DIVIDE;
            case Key::kp_multiply:
                return SDL_SCANCODE_KP_MULTIPLY;
            case Key::kp_subtract:
                return SDL_SCANCODE_KP_MINUS;
            case Key::kp_add:
                return SDL_SCANCODE_KP_PLUS;
            case Key::kp_enter:
                return SDL_SCANCODE_KP_ENTER;
            case Key::kp_equal:
                return SDL_SCANCODE_KP_EQUALS;
            case Key::back:
                return SDL_SCANCODE_AC_BACK;
            case Key::volume_up:
                return SDL_SCANCODE_VOLUMEUP;
            case Key::volume_down:
                return SDL_SCANCODE_VOLUMEDOWN;
            default:
                return SDL_SCANCODE_UNKNOWN;
        }
    }

}  // namespace ugfx::sdl
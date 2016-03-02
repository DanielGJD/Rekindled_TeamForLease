/*!
    \file   Keys.h
    \author Christopher Hudson
    \date   09/22/15

    \brief
        Defines key codes and key states, a direct mapping to SDL key key codes.

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef KEYS_H
#define KEYS_H

#include <SDL.h>
#include <vector>
#include <string>
#include <unordered_map>

namespace ForLeaseEngine {
    /*!
        \class Keys

        \brief
            Definition of all key codes
    */
    class Keys {
    public:
        // Numbers
        static const int One   = SDLK_1;
        static const int Two   = SDLK_2;
        static const int Three = SDLK_3;
        static const int Four  = SDLK_4;
        static const int Five  = SDLK_5;
        static const int Six   = SDLK_6;
        static const int Seven = SDLK_7;
        static const int Eight = SDLK_8;
        static const int Nine  = SDLK_9;
        static const int Zero  = SDLK_0;

        // Numberpad Numbers
        static const int NumpadZero  = SDLK_KP_0;
        static const int NumpadOne   = SDLK_KP_1;
        static const int NumpadTwo   = SDLK_KP_2;
        static const int NumpadThree = SDLK_KP_3;
        static const int NumpadFour  = SDLK_KP_4;
        static const int NumpadFive  = SDLK_KP_5;
        static const int NumpadSix   = SDLK_KP_6;
        static const int NumpadSeven = SDLK_KP_7;
        static const int NumpadEight = SDLK_KP_8;
        static const int NumpadNine  = SDLK_KP_9;

        // Numberpad Symbols
        static const int NumpadPlus     = SDLK_KP_PLUS;
        static const int NumpadMultiply = SDLK_KP_MULTIPLY;
        static const int NumpadMinus    = SDLK_KP_MINUS;
        static const int NumpadDivide   = SDLK_KP_DIVIDE;
        static const int NumpadPeriod   = SDLK_KP_PERIOD;

        // Letters
        static const int A = SDLK_a;
        static const int B = SDLK_b;
        static const int C = SDLK_c;
        static const int D = SDLK_d;
        static const int E = SDLK_e;
        static const int F = SDLK_f;
        static const int G = SDLK_g;
        static const int H = SDLK_h;
        static const int I = SDLK_i;
        static const int J = SDLK_j;
        static const int K = SDLK_k;
        static const int L = SDLK_l;
        static const int M = SDLK_m;
        static const int N = SDLK_n;
        static const int O = SDLK_o;
        static const int P = SDLK_p;
        static const int Q = SDLK_q;
        static const int R = SDLK_r;
        static const int S = SDLK_s;
        static const int T = SDLK_t;
        static const int U = SDLK_u;
        static const int V = SDLK_v;
        static const int W = SDLK_w;
        static const int X = SDLK_x;
        static const int Y = SDLK_y;
        static const int Z = SDLK_z;

        // F keys
        static const int F1  = SDLK_F1;
        static const int F2  = SDLK_F2;
        static const int F3  = SDLK_F3;
        static const int F4  = SDLK_F4;
        static const int F5  = SDLK_F5;
        static const int F6  = SDLK_F6;
        static const int F7  = SDLK_F7;
        static const int F8  = SDLK_F8;
        static const int F9  = SDLK_F9;
        static const int F10 = SDLK_F10;
        static const int F11 = SDLK_F11;
        static const int F12 = SDLK_F12;

        // Arrow keys
        static const int Up    = SDLK_UP;
        static const int Right = SDLK_RIGHT;
        static const int Down  = SDLK_DOWN;
        static const int Left  = SDLK_LEFT;

        // Misc
        static const int Tab          = SDLK_TAB;
        static const int CapsLock     = SDLK_CAPSLOCK;
        static const int NumLock      = SDLK_NUMLOCKCLEAR;
        static const int ScrollLock   = SDLK_SCROLLLOCK;
        static const int LeftShift    = SDLK_LSHIFT;
        static const int RightShift   = SDLK_RSHIFT;
        static const int LeftControl  = SDLK_LCTRL;
        static const int RightControl = SDLK_RCTRL;
        static const int LeftAlt      = SDLK_LALT;
        static const int RightAlt     = SDLK_RALT;
        static const int Backspace    = SDLK_BACKSPACE;
        static const int Return       = SDLK_RETURN;
        static const int NumpadEnter  = SDLK_KP_ENTER;
        static const int Insert       = SDLK_INSERT;
        static const int Delete       = SDLK_DELETE;
        static const int PageUp       = SDLK_PAGEUP;
        static const int PageDown     = SDLK_PAGEDOWN;
        static const int Escape       = SDLK_ESCAPE;
        static const int Space        = SDLK_SPACE;

        static const int BackQuote    = SDLK_BACKQUOTE;
        static const int Minus        = SDLK_MINUS;
        static const int Equals       = SDLK_EQUALS;
        static const int LeftBracket  = SDLK_LEFTBRACKET;
        static const int RightBracket = SDLK_RIGHTBRACKET;
        static const int Semicolon    = SDLK_SEMICOLON;
        static const int Quote        = SDLK_QUOTE;
        static const int Comma        = SDLK_COMMA;
        static const int Period       = SDLK_PERIOD;
        static const int BackSlash    = SDLK_BACKSLASH;
        static const int Slash        = SDLK_SLASH;

        static void InitKeymap();
        static std::vector<std::string> GetKeyStrings();
        static int GetKeyFromString(std::string const& key);
    private:
        static std::unordered_map<std::string, int> StringToKeycode;

        Keys();
    };

    /*!
        \class KeyState

        \brief
            Definition of key states
    */
    class KeyState {
    public:
        static const int Pressed = SDL_PRESSED;
        static const int Released = SDL_RELEASED;
    };
}

#endif // KEYS_H

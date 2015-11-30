/*!
    \file   Mouse.h
    \author Christopher Hudson

    \brief
        Defines a class containing information on mouse buttons and states

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef MOUSE_H
#define MOUSE_H

#include <SDL.h>

namespace ForLeaseEngine {
    class MouseButtonState {
        public:
            static const int Pressed = SDL_PRESSED;
            static const int Released = SDL_RELEASED;
    };

    class MouseButton {
        public:
            static const int Left = SDL_BUTTON_LEFT;
            static const int Middle = SDL_BUTTON_MIDDLE;
            static const int Right = SDL_BUTTON_RIGHT;
            static const int ButtonFour = SDL_BUTTON_X1;
            static const int ButtonFive = SDL_BUTTON_X2;
    };
}

#endif // MOUSE_H

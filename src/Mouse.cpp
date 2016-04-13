/*!
    \file   Mouse.cpp
    \author Christopher Hudson

    \brief
        Defines a functions for setting the visibility of the mouse cursor

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "Mouse.h"

namespace ForLeaseEngine {
    namespace Mouse {
        void SetMouseVisible(bool visible) {
            if(visible) {
                SDL_ShowCursor(SDL_ENABLE);
            }
            else {
                SDL_ShowCursor(SDL_DISABLE);
            }
        }

        void ToggleMouseVisible() {
            int state = SDL_ShowCursor(SDL_QUERY);
            SetMouseVisible(state == SDL_DISABLE);
        }
    }
}

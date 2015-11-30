/*!
    \file   Input.h
    \author Christopher Hudson
    \date   09/22/15

    \brief
        Defines a class to process OS events

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/
#ifndef INPUT_H
#define INPUT_H

#include "EventDispatcher.h"
#include "Window.h"
#include <SDL.h>

namespace ForLeaseEngine
{
    /*!
        \class Input

        \brief
            Class that processes OS events
    */
    class Input {
        public:
            Input();
            ~Input();


            void ProcessAllInput();
            //void ProcessInput(int events);
            void ProcessAllInputWithImgui();

            //! For testing only, do not use
            EventDispatcher* dispatcher;
            Systems::Window* GameWindow;
        private:
            void ProcessEvent(SDL_Event& SDL_e);
    };
}

#endif // INPUT_H

/*!
    \file   Input.cpp
    \author Christopher Hudson
    \date   09/22/15

    \brief
        Implementation of a class that manages user input

    \see KeyboardEvent.h
*/
#include <SDL.h>
#include "Keys.h"
#include "KeyboardEvent.h"
#include "Input.h"

namespace ForLeaseEngine {
    /*!
        \brief
            Creates the Input object and initializes the OS interface
    */
    Input::Input() {
        SDL_InitSubSystem(SDL_INIT_EVENTS);
    }

    /*!
        \brief
            Closes the OS interface
    */
    Input::~Input() {
        SDL_QuitSubSystem(SDL_INIT_EVENTS);
    }

    /*!
        \brief
            Process all OS events in the event queue and resends the events
    */
    void Input::ProcessAllInput() {
        SDL_Event SDL_e;

        while(SDL_PollEvent(&SDL_e)) {
            switch(SDL_e.type) {
            case SDL_KEYDOWN:
                if(!SDL_e.key.repeat) {
                    KeyboardEvent e = KeyboardEvent("KeyDown", SDL_e.key.keysym.sym, SDL_e.key.state);
                    dispatcher->Dispatch(&e, this);
                }
                break;
            case SDL_KEYUP:
                if(!SDL_e.key.repeat) {
                    KeyboardEvent e = KeyboardEvent("KeyUp", SDL_e.key.keysym.sym, SDL_e.key.state);
                    dispatcher->Dispatch(&e, this);
                }
                break;
            }
        }
    }
}

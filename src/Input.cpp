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
#include "MouseButtonEvent.h"
#include "MouseMotionEvent.h"
#include "Mouse.h"
#include "Input.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"

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
            ProcessEvent(SDL_e);
        }
    }

    void Input::ProcessAllInputWithImgui() {
        SDL_Event SDL_e;
        while(SDL_PollEvent(&SDL_e)) {
            if(!ImGui_ImplSdl_ProcessEvent(&SDL_e)) {
                ProcessEvent(SDL_e);
            }
        }
    }

    void Input::ProcessEvent(SDL_Event& SDL_e) {
        switch(SDL_e.type) {
            // Keyboard Events
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

            // Mouse Events
            case SDL_MOUSEBUTTONDOWN:
                if(SDL_e.button.which != SDL_TOUCH_MOUSEID) {
                    MouseButtonEvent e = MouseButtonEvent("MouseButtonDown",
                                                          SDL_e.button.button,
                                                          SDL_e.button.state,
                                                          SDL_e.button.clicks,
                                                          Point(SDL_e.button.x, GameWindow->GetYResolution() - SDL_e.button.y));
                    dispatcher->Dispatch(&e, this);
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if(SDL_e.button.which != SDL_TOUCH_MOUSEID) {
                    MouseButtonEvent e = MouseButtonEvent("MouseButtonUp",
                                                          SDL_e.button.button,
                                                          SDL_e.button.state,
                                                          SDL_e.button.clicks,
                                                          Point(SDL_e.button.x, GameWindow->GetYResolution() - SDL_e.button.y));
                    dispatcher->Dispatch(&e, this);
                }
                break;

            case SDL_MOUSEMOTION:
                MouseMotionEvent e = MouseMotionEvent("MouseMotion",
                                                      SDL_e.motion.x,
                                                      GameWindow->GetYResolution() - SDL_e.motion.y,
                                                      SDL_e.motion.xrel,
                                                      -SDL_e.motion.yrel);
                dispatcher->Dispatch(&e, this);
                break;

        }
    }
}

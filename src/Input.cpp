/*!
    \file   Input.cpp
    \author Christopher Hudson
    \date   09/22/15

    \brief
        Implementation of a class that dispatches OS events

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved

    \see KeyboardEvent.h
*/
#include <SDL.h>
#include "Keys.h"
#include "KeyboardEvent.h"
#include "MouseButtonEvent.h"
#include "MouseMotionEvent.h"
#include "WindowEvent.h"
#include "Mouse.h"
#include "Input.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "Engine.h"

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
            /*if(!ImGui_ImplSdl_ProcessEvent(&SDL_e)) {
                ProcessEvent(SDL_e);
            }*/
            ImGui_ImplSdl_ProcessEvent(&SDL_e);
            if(SDL_e.type == SDL_QUIT) {
                ProcessEvent(SDL_e);
            }
        }
    }

    void Input::ProcessEvent(SDL_Event& SDL_e) {
        switch(SDL_e.type) {
            case SDL_QUIT:
                ForLease->GameStateManager().SetAction(Modules::StateAction::Quit);
                break;

            // Keyboard Events
            case SDL_KEYDOWN:
                if(!SDL_e.key.repeat) {
                    KeyboardEvent e = KeyboardEvent("KeyDown", SDL_e.key.keysym.sym, SDL_e.key.state);
                    //std::cout << "KeyDown:" << e.Key << std::endl;
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
            {
                if(SDL_e.button.which != SDL_TOUCH_MOUSEID) {
                    MouseButtonEvent e = MouseButtonEvent("MouseButtonDown",
                                                          SDL_e.button.button,
                                                          SDL_e.button.state,
                                                          SDL_e.button.clicks,
                                                          Point(SDL_e.button.x, GameWindow->GetYResolution() - SDL_e.button.y));
                    dispatcher->Dispatch(&e, this);
                }
                break;
            }
            case SDL_MOUSEBUTTONUP:
            {
                if(SDL_e.button.which != SDL_TOUCH_MOUSEID) {
                    MouseButtonEvent e = MouseButtonEvent("MouseButtonUp",
                                                          SDL_e.button.button,
                                                          SDL_e.button.state,
                                                          SDL_e.button.clicks,
                                                          Point(SDL_e.button.x, GameWindow->GetYResolution() - SDL_e.button.y));
                    dispatcher->Dispatch(&e, this);
                }
                break;
            }
            case SDL_MOUSEMOTION:
            {
                MouseMotionEvent e = MouseMotionEvent("MouseMotion",
                                                      SDL_e.motion.x,
                                                      GameWindow->GetYResolution() - SDL_e.motion.y,
                                                      SDL_e.motion.xrel,
                                                      -SDL_e.motion.yrel);
                dispatcher->Dispatch(&e, this);
                break;
            }
            case SDL_WINDOWEVENT:
            {
                switch(SDL_e.window.event) {
                    case SDL_WINDOWEVENT_SHOWN:
                        std::cout << "Window " << SDL_e.window.windowID << " shown" << std::endl;
                        break;
                    case SDL_WINDOWEVENT_HIDDEN:
                        std::cout << "Window " << SDL_e.window.windowID << " hidden" << std::endl;
                        break;
                    case SDL_WINDOWEVENT_EXPOSED:
                    {
                        std::cout << "Window " << SDL_e.window.windowID << " exposed" << std::endl;
                        WindowEvent e = WindowEvent(WindowEvent::Exposed);
                        ForLease->Dispatcher.Dispatch(&e, this);
                        break;
                    }
                    case SDL_WINDOWEVENT_MOVED:
                        std::cout << "Window " << SDL_e.window.windowID << " moved to (" << SDL_e.window.data1 << "," << SDL_e.window.data2 << ")" << std::endl;
                        break;
                    case SDL_WINDOWEVENT_RESIZED:
                        std::cout << "Window " << SDL_e.window.windowID << " resized to (" << SDL_e.window.data1 << "," << SDL_e.window.data2 << ")" << std::endl;
                        break;
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        std::cout << "Window " << SDL_e.window.windowID << " size changed to (" << SDL_e.window.data1 << "," << SDL_e.window.data2 << ")" << std::endl;
                        break;
                    case SDL_WINDOWEVENT_MINIMIZED:
                        std::cout << "Window " << SDL_e.window.windowID << " minimized" << std::endl;
                        break;
                    case SDL_WINDOWEVENT_MAXIMIZED:
                        std::cout << "Window " << SDL_e.window.windowID << " maximized" << std::endl;
                        break;
                    case SDL_WINDOWEVENT_RESTORED:
                        std::cout << "Window " << SDL_e.window.windowID << " restored" << std::endl;
                        break;
                    case SDL_WINDOWEVENT_ENTER:
                        std::cout << "Window " << SDL_e.window.windowID << " gained mouse focus" << std::endl;
                        break;
                    case SDL_WINDOWEVENT_LEAVE:
                        std::cout << "Window " << SDL_e.window.windowID << " lost mouse focus" << std::endl;
                        break;
                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                    {
                        std::cout << "Window " << SDL_e.window.windowID << " gained keyboard focus" << std::endl;
                        WindowEvent e = WindowEvent(WindowEvent::FocusGained);
                        ForLease->Dispatcher.Dispatch(&e, this);
                        break;
                    }
                    case SDL_WINDOWEVENT_FOCUS_LOST:
                    {
                        std::cout << "Window " << SDL_e.window.windowID << " lost keyboard focus" << std::endl;
                        WindowEvent e = WindowEvent(WindowEvent::FocusLost);
                        ForLease->Dispatcher.Dispatch(&e, this);
                        break;
                    }
                    case SDL_WINDOWEVENT_CLOSE:
                        std::cout << "Window " << SDL_e.window.windowID << " closed" << std::endl;
                        break;
                }
            }

        }
    }
}

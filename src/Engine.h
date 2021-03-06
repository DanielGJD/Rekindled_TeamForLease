/*!
    \file   Engine.h
    \author Sean McGeer
    \date   9/9/15
    \brief
        Defines the Engine class.
    \see Engine.cpp

    \copyright ęCopyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef ENGINE_H
#define ENGINE_H

#include "GameStateManager.h"
#include "FrameRateController.h"
#include "Window.h"
#include "Input.h"
#include "EventDispatcher.h"
#include "ResourceManager.h"
#include "SoundFS.h"
#include "Filesystem.h"
#include "Debug.h"
#include <vector>

namespace ForLeaseEngine {

    /*!
        \class Engine
        \brief
            The base of the game engine.
    */
    class Engine {

        public:
            //Engine(std::vector<State *> states);
            Engine(std::vector<State *> states, int resolutionX = 1280, int resolutionY = 720, int frameRate = 60, bool fullscreen = false, Modules::Debug debug = Modules::Debug());
            void Run();
            Modules::GameStateManager& GameStateManager();
            Modules::FrameRateController& FrameRateController();
            Systems::Window* GameWindow;
            Modules::ResourceManager Resources;
            ForLeaseEngine::EventDispatcher Dispatcher;
            ForLeaseEngine::Input OSInput;
            Systems::SoundManager* sound;
            Modules::Filesystem Filesystem;
            Modules::Debug Debug;
        private:
            //! Width of the screen
            int ResolutionX;
            //! Height of the screen
            int ResolutionY;
            //! Frame rate of the game
            int FrameRate;
            //! Whether it's fullscreen
            bool Fullscreen;
            //! Game state manager--handles levels and switching between them
            Modules::GameStateManager GSM;
            //! Frame rate controller--handles timing
            Modules::FrameRateController FRC;

            Engine() = delete;
    };

} // ForLeaseEngine

extern ForLeaseEngine::Engine * ForLease;

#endif

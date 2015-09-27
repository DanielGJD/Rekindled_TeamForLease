/*!
    \file   Engine.h
    \author Sean McGeer
    \date   9/9/15
    \brief
        Defines the Engine class.
    \see Engine.cpp
*/

#ifndef ENGINE_H
#define ENGINE_H

#include "GameStateManager.h"
#include "FrameRateController.h"
#include "Graphics.h"
#include "Input.h"
#include "EventDispatcher.h"
#include <vector>

namespace ForLeaseEngine {

    /*!
        \class Engine
        \brief
            The base of the game engine.
    */
    class Engine {

        public:
            Engine(std::vector<State *> states);
            Engine(std::vector<State *> states, int resolutionX, int resolutionY, int frameRate);
            void Run();
            Modules::GameStateManager& GameStateManager();
            Modules::FrameRateController& FrameRateController();
            ForLeaseEngine::Graphics* Window;
            ForLeaseEngine::EventDispatcher Dispatcher;
            ForLeaseEngine::Input OSInput;
        private:
            //! Width of the screen
            int ResolutionX;
            //! Height of the screen
            int ResolutionY;
            //! Frame rate of the game
            int FrameRate;
            //! Game state manager--handles levels and switching between them
            Modules::GameStateManager GSM;
            //! Frame rate controller--handles timing
            Modules::FrameRateController FRC;

            Engine() = delete;
    };

} // ForLeaseEngine

extern ForLeaseEngine::Engine * ForLease;

#endif

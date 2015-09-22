/*!
    \file   Engine.h
    \author Sean McGeer
    \date   9/9/15
    \brief
        Defines the Engine class.
    \see Engine.cpp
*/

namespace ForLeaseEngine {
    class Engine;
}

#ifndef ENGINE_H
#define ENGINE_H

#include "GameStateManager.h"
#include "FrameRateController.h"

namespace ForLeaseEngine {

    /*!
        \class Engine
        \brief
            The base of the game engine.
    */
    class Engine {

        public:
            Engine();
            Engine(int resolutionX, int resolutionY, int frameRate);
            void Run();
            Modules::GameStateManager& GameStateManager();
            Modules::FrameRateController& FrameRateController();
        private:
            //! Width of the screen
            int ResolutionX;
            //! Height of the screen
            int ResolutionY;
            //! Frame rate of the game
            int FrameRate;
            Modules::GameStateManager GSM;
            Modules::FrameRateController FRC;

    };

} // ForLeaseEngine

#endif

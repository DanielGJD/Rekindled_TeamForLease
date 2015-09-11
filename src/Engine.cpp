/*!
    \file   Engine.cpp
    \author Sean McGeer
    \date   9/9/15
    \brief
        Contains implementations for the Engine class defined in Engine.h.
    \see Engine.h
*/

#include "Engine.h"

/*!
    \namespace ForLeaseEngine
    \brief
        The wrapper namespace for everything involved in the For Lease %Engine.
*/
namespace ForLeaseEngine {

    /*!
        Constructor for Engine.  Creates a new instance of Engine using default values.
    */
    Engine::Engine() {
        // Default values
        resolutionX = 1280;
        resolutionY = 720;
        frameRate   = 60;
    }

    /*!
        Constructor for Engine.  Creates a new instance of Engine using given values.
        
        \param rx
            The width (x-resolution) of the window to run in.
        \param ry
            The height (y-resolution) of the window to run in.
        \param fr
            The frame rate, or at which to refresh the window.
    */
    Engine::Engine(int rx, int ry, int fr) : resolutionX(rx), resolutionY(ry), frameRate(fr) {}

    /*!
        Handles the main game loop.  Essentially calls FrameRateController and GameStateManager.
    */
    void Engine::Run() {
        // Empty for now, as nothing has been implemented.
    }

} // ForLeaseEngine

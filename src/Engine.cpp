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
        ResolutionX = 800;
        ResolutionY = 600;
        FrameRate   = 60;
    }

    /*!
        Constructor for Engine.  Creates a new instance of Engine using given values.
        
        \param resolutionX
            The width (x-resolution) of the window to run in.
        \param resolutionY
            The height (y-resolution) of the window to run in.
        \param frameRate
            The frame rate, or at which to refresh the window.
    */
    Engine::Engine(int resolutionX, int resolutionY, int frameRate)
        : ResolutionX(resolutionX), ResolutionY(resolutionY),
          FrameRate(frameRate) {}

    /*!
        Handles the main game loop.  Essentially calls FrameRateController and GameStateManager.
    */
    void Engine::Run() {
        // Empty for now, as nothing has been implemented.
    }

} // ForLeaseEngine

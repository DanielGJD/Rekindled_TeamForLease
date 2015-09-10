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
        
        \param resolutionX
            The width of the window to run in.
        \param resolutionY
            The height of the window to run in.
        \param frameRate
            The rate at which to refresh the window.
    */

    Engine::Engine(int resolutionX, int resolutionY, int frameRate) : resolutionX(resolutionX), resolutionY(resolutionY), frameRate(frameRate) {}

} // ForLeaseEngine
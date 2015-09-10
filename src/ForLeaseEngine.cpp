/*!
    \file   ForLeaseEngine.cpp
    \author Sean McGeer
    \date   9/9/15
    \brief
        Contains implementations for the ForLeaseEngine class defined in ForLeaseEngine.h.
*/

#include "ForLeaseEngine.h"

/*!
    Constructor for ForLeaseEngine.  Creates a new instance of ForLeaseEngine using default values.
*/
ForLeaseEngine::ForLeaseEngine() {
    // Default values
    resolutionX = 1280;
    resolutionY = 720;
    frameRate   = 60;
}

/*!
    Constructor for ForLeaseEngine.  Creates a new instance of ForLeaseEngine using given values.
    
    \param resolutionX
        The width of the window to run in.
    \param resolutionY
        The height of the window to run in.
    \param frameRate
        The rate at which to refresh the window.
*/

ForLeaseEngine::ForLeaseEngine(int resolutionX, int resolutionY, int frameRate) : resolutionX(resolutionX), resolutionY(resolutionY), frameRate(frameRate) {}
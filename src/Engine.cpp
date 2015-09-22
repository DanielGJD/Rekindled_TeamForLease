/*!
    \file   Engine.cpp
    \author Sean McGeer
    \date   9/9/15
    \brief
        Contains implementations for the Engine class defined in Engine.h.
    \see Engine.h
*/

#include "Engine.h"

ForLeaseEngine::Engine * ForLease;

/*!
    \namespace ForLeaseEngine
    \brief
        The wrapper namespace for everything involved in the For Lease %Engine.
*/
namespace ForLeaseEngine {

    /*!
        Constructor for Engine.  Creates a new instance of Engine using default values.
    */
    Engine::Engine(std::vector<State *> states) : GSM(*this, states), FRC(*this) {
        // Default values
        ResolutionX = 800;
        ResolutionY = 600;
        FrameRate   = 60;

        ForLease = this;
    }

    /*!
        Constructor for Engine.  Creates a new instance of Engine using given values.
        
        \param states
            A vector of all states in the game.  This gets passed to the game
            state manager.
        \param resolutionX
            The width (x-resolution) of the window to run in.
        \param resolutionY
            The height (y-resolution) of the window to run in.
        \param frameRate
            The frame rate, or at which to refresh the window.
    */
    Engine::Engine(std::vector<State *> states, int resolutionX, int resolutionY,
        int frameRate) : ResolutionX(resolutionX), ResolutionY(resolutionY),
        FrameRate(frameRate), GSM(*this, states), FRC(*this) { ForLease = this; }

    /*!
        Handles the main game loop.  Essentially calls FrameRateController and GameStateManager.
    */
    void Engine::Run() {
        GSM.Run();
    }

    /*!
        Returns a reference to the current GameStateManager used by the engine.
        \return
            A reference to the GameStateManager used by the engine.
    */
    Modules::GameStateManager& Engine::GameStateManager() { return GSM; }

    /*!
        Returns a reference to the current FrameRateController used by the
        engine.
        \return
            A reference to the FrameRateController used by the engine.
    */
    Modules::FrameRateController& Engine::FrameRateController() { return FRC; }

} // ForLeaseEngine

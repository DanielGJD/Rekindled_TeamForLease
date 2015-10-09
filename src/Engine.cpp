/*!
    \file   Engine.cpp
    \author Sean McGeer
    \date   9/9/15
    \brief
        Contains implementations for the Engine class defined in Engine.h.
    \see Engine.h
*/

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "Engine.h"
#include "Window.h"

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
    Engine::Engine(std::vector<State *> states) : GSM(*this, states), FRC(60) {
        // Default values
        ResolutionX = 1280;
        ResolutionY = 720;
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
        FrameRate(frameRate), GSM(*this, states), FRC(frameRate) { ForLease = this; }

    /*!
        Handles the main game loop.  Essentially calls FrameRateController and GameStateManager.
    */
    void Engine::Run() {
        SDL_Init(0);
        Systems::WindowProperties properties = Systems::WindowProperties();
        properties.xResolution = ResolutionX;
        properties.yResolution = ResolutionY;
        properties.fullscreen = false;
        GameWindow = Systems::Window::CreateGameWindow(properties);
        OSInput.dispatcher = &Dispatcher;
        GSM.Run();
        Systems::Window::DestroyGameWindow(GameWindow);
        SDL_Quit();
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

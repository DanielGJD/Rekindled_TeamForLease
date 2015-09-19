/*!
    \file   GameStateManager.cpp
    \author Sean McGeer
    \date   9/14/15
    \brief
        Contains implementations for the GameStateManager class defined in GameStateManager.h.
    \see GameStateManager.h
*/

#include "GameStateManager.h"

namespace ForLeaseEngine {

    /*!
        Constructor for GameStateManager.  Creates a new instance of GameStateManager using default values.
    */
    GameStateManager::GameStateManager() {
        // Default values
        Previous = slContinue;
        Current = slContinue;
        Next = slContinue;
    }

    /*!
        Handles the main game loop.  Calls the current state's Update function.
    */
    void GameStateManager::Update() {
        // Empty for now
    }

} // ForLeaseEngine

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

    namespace Modules {
        /*!
            Constructor for GameStateManager.  Creates a new instance of GameStateManager using default values.
        */
        // GameStateManager::GameStateManager() {
        // }

        /*!
            Constructor for GameStateManager.  Creates a new instance of GameStateManager using a provided Engine as its parent.
        */
        GameStateManager::GameStateManager(const Engine& parent) : Parent(parent) {}

        /*!
            Constructor for GameStateManager.  Creates a new instance of GameStateManager using a provided State list.
        */
        GameStateManager::GameStateManager(const Engine& parent, std::vector<State *> states)
            : Parent(parent), States(states) {}

        /*!
            Handles the main game loop.  Calls the current state's Update function.
        */
        void GameStateManager::Update() {
            for (State* state : States) {
                Parent.FrameRateController().Start();
                state->Update();
                Parent.FrameRateController().End();
            }
        }
    }

} // ForLeaseEngine

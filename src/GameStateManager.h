/*!
    \file   GameStateManager.h
    \author Sean McGeer
    \date   9/14/15
    \brief
        Defines the GameStateManager class.
    \see GameStateManager.cpp
*/

#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

#include <vector>
#include "State.h"
#include "Engine.h"

namespace ForLeaseEngine {

    namespace Modules {
        /*!
            \class GameStateManager
            \brief
                A state manager to switch between game states (levels, main menu, and so forth).
        */
        class GameStateManager {

            public:
                // GameStateManager();
                GameStateManager(Engine& parent);
                GameStateManager(Engine& parent, std::vector<State *> states);
                void Update();
            private:
                //! A reference to the engine
                Engine& Parent;
                // //! The previous state
                // STATE_LIST Previous;
                // //! The current state
                // STATE_LIST Current;
                // //! The next state
                // STATE_LIST Next;
                //! The vector of all possible states
                std::vector<State *> States;

                GameStateManager() = delete;

        };
    }

} // ForLeaseEngine

#endif

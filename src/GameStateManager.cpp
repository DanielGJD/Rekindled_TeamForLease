/*!
    \file   GameStateManager.cpp
    \author Sean McGeer
    \date   9/14/15
    \brief
        Contains implementations for the GameStateManager class defined in GameStateManager.h.
    \see GameStateManager.h
*/

#include "GameStateManager.h"
#include "Engine.h"

namespace ForLeaseEngine {

    namespace Modules {

        /*!
            Constructor for GameStateManager.  Creates a new instance of GameStateManager using a provided Engine as its parent.
        */
        GameStateManager::GameStateManager(Engine& parent)
            : Parent(parent), StateIndex(0), Action(StateAction::Continue) {}

        /*!
            Constructor for GameStateManager.  Creates a new instance of GameStateManager using a provided State list.
        */
        GameStateManager::GameStateManager(Engine& parent, std::vector<State *> states)
            : Parent(parent), States(states), StateIndex(0),
              Action(StateAction::Continue) {}

        /*!
            Handles the main game loop.  Calls the current state's Update function.
        */
        void GameStateManager::Run() {
            while (Action != StateAction::Quit) {
                States[StateIndex]->Load();

                do {

                    States[StateIndex]->Initialize();

                    Action = StateAction::Continue;

                    do {
                        Parent.FrameRateController().Start();
                        States[StateIndex]->Update();
                        Parent.FrameRateController().End();
                    } while (Action == StateAction::Continue);

                    States[StateIndex]->Deinitialize();

                } while (Action == StateAction::Restart);

                States[StateIndex]->Unload();
                ++StateIndex;
            }
        }

        /*!
            Sets the action of the GameStateManager.
            \param action
                A StateAction that we'll set the Action of the GameStateManger to.
        */
        void GameStateManager::SetAction(StateAction action) { Action = action; }

    }

} // ForLeaseEngine

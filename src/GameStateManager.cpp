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
#include "State.h"

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
                States[StateIndex]->Load(); // Load the next state

                do {

                    States[StateIndex]->Initialize(); // Initialize the current state

                    Action = StateAction::Continue; // Set the action so we won't get booted unless otherwise told

                    do {

                        Parent.FrameRateController().Start(); // Start the current frame
                        States[StateIndex]->Update();         // Do all the game stuff
                        Parent.FrameRateController().End();   // End the current frame

                    } while (Action == StateAction::Continue);

                    States[StateIndex]->Deinitialize(); // Deinitialize the state

                } while (Action == StateAction::Restart);

                States[StateIndex]->Unload(); // Free memory allocated by the state

                if (Action == StateAction::Next) ++StateIndex; // If we're just going to the next level, increment the state index
                if (Action == StateAction::Skip) StateIndex = NextStateIndex; // Go to the next requested level
            }
        }

        /*!
            Sets the action of the GameStateManager.
            \param action
                A StateAction that we'll set the Action of the GameStateManger to.
        */
        void GameStateManager::SetAction(StateAction action) { Action = action; }

        /*!
            Sets the state of the GameStateManager.

            \param stateName
                The name of the state to skip to.
        */
        void GameStateManager::SetState(std::string stateName) {
            for (unsigned i = 0; i < States.size(); ++i)
                if (States[i]->GetName() == stateName) {
                    NextStateIndex = i;
                    break;
                }
        }

        /*!
            Sets the state of the GameStateManager.

            \param stateIndex
                An unsigned int that is the index of the state we want to skip to.
        */
        void GameStateManager::SetState(unsigned stateIndex) {
            NextStateIndex = stateIndex;
        }
        
        /*!
            Get the currently loaded state.
            
            \return
                A reference to the current state.
        */
        State& GameStateManager::CurrentState() {
            return *States[StateIndex];
        }

    }

} // ForLeaseEngine

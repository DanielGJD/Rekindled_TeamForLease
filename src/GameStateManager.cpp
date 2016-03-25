/*!
    \file   GameStateManager.cpp
    \author Sean McGeer
    \date   9/14/15
    \brief
        Contains implementations for the GameStateManager class defined in GameStateManager.h.
    \see GameStateManager.h

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "GameStateManager.h"
#include "Engine.h"
#include "State.h"
#include "PauseMenu.h"

namespace ForLeaseEngine {

    namespace Modules {

        /*!
            Constructor for GameStateManager.  Creates a new instance of GameStateManager using a provided Engine as its parent.
        */
        GameStateManager::GameStateManager(Engine& parent)
            : Parent(parent), StateIndex(0), Action(StateAction::Continue), UnfreezeAction(StateAction::Continue) {}

        /*!
            Constructor for GameStateManager.  Creates a new instance of GameStateManager using a provided State list.
        */
        GameStateManager::GameStateManager(Engine& parent, std::vector<State *> states)
            : Parent(parent), States(states), StateIndex(0),
              Action(StateAction::Continue), UnfreezeAction(StateAction::Continue) {}

        void GameStateManager::Initialize() {
            ForLease->Dispatcher.Attach(NULL, this, "FocusGained", &GameStateManager::FocusUnfreeze);
            ForLease->Dispatcher.Attach(NULL, this, "FocusLost", &GameStateManager::UnfocusFreeze);
            PauseScreen = new PauseMenu();
            //PauseScreen->Initialize();
        }

        /*!
            Handles the main game loop.  Calls the current state's Update function.
        */
        void GameStateManager::Run() {
            while (Action != StateAction::Quit) {
                StateCurrentlyExecuting = States[StateIndex];
                States[StateIndex]->Load(); // Load the next state

                do {

                    States[StateIndex]->Initialize(); // Initialize the current state

                    Action = StateAction::Continue; // Set the action so we won't get booted unless otherwise told

                    bool frozenLastFrame = false;

                    do {
                        Parent.FrameRateController().Start();   // Start the current frame
                        if (!frozenLastFrame) {
                            States[StateIndex]->Update();       // Do all the game stuff
                        }
                        else {
                            frozenLastFrame = false;
                        }
                        Parent.FrameRateController().End();     // End the current frame

                        while (Action == StateAction::Freeze) {
                            ForLease->OSInput.ProcessAllInput();
                            Parent.FrameRateController().SleepFor(0.1);
                            frozenLastFrame = true;
                        }

                        if (Action == StateAction::Pause) {

                            StateCurrentlyExecuting = PauseScreen;

                            PauseScreen->Load();
                            PauseScreen->Initialize();

                            while (Action == StateAction::Pause) {
                                Parent.FrameRateController().Start();
                                PauseScreen->Update();
                                Parent.FrameRateController().End();

                                while (Action == StateAction::Freeze) {
                                    ForLease->OSInput.ProcessAllInput();
                                    Parent.FrameRateController().SleepFor(0.1);
                                    frozenLastFrame = true;
                                }
                            }

                            PauseScreen->Deinitialize();
                            PauseScreen->Unload();

                            StateCurrentlyExecuting = States[StateIndex];

                        }

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
            std::cout << "===" << std::endl;
            for (unsigned i = 0; i < States.size(); ++i) {
                std::cout << States[i]->GetName() << std::endl;
                if (States[i]->GetName() == stateName) {
                    NextStateIndex = i;
                    SetAction(StateAction::Skip);
                    std::cout << "Found state " << stateName << std::endl;
                    break;
                }
            }
        }

        /*!
            Sets the state of the GameStateManager.

            \param stateIndex
                An unsigned int that is the index of the state we want to skip to.
        */
        void GameStateManager::SetState(unsigned stateIndex) {
            NextStateIndex = stateIndex;
            SetAction(StateAction::Skip);
        }

        void GameStateManager::UnfocusFreeze(const Event* e) {
            UnfreezeAction = Action;
            Action = StateAction::Freeze;
        }

        void GameStateManager::FocusUnfreeze(const Event* e) {
            Action = UnfreezeAction;
        }

        /*!
            Get the currently loaded state.

            \return
                A reference to the current state.
        */
        State& GameStateManager::CurrentState() {
            return *StateCurrentlyExecuting;
        }

        unsigned GameStateManager::NumLevels() {
            return States.size();
        }

    }

} // ForLeaseEngine

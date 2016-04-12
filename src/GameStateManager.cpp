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
#include "WindowEvent.h"

namespace ForLeaseEngine {

    namespace Modules {

        /*!
            Constructor for GameStateManager.  Creates a new instance of GameStateManager using a provided Engine as its parent.
        */
        GameStateManager::GameStateManager(Engine& parent)
            : Parent(parent), StateIndex(0), Action(StateAction::Continue), UnfreezeAction(StateAction::Continue), FreezeCount(0) {}

        /*!
            Constructor for GameStateManager.  Creates a new instance of GameStateManager using a provided State list.
        */
        GameStateManager::GameStateManager(Engine& parent, std::vector<State *> states)
            : Parent(parent), States(states), StateIndex(0),
              Action(StateAction::Continue), UnfreezeAction(StateAction::Continue), FreezeCount(0) {}

        void GameStateManager::Initialize() {
            ForLease->Dispatcher.Attach(NULL, this, WindowEvent::FocusGained, &GameStateManager::FocusUnfreeze);
            ForLease->Dispatcher.Attach(NULL, this, WindowEvent::FocusLost, &GameStateManager::UnfocusFreeze);
            ForLease->Dispatcher.Attach(NULL, this, WindowEvent::Exposed, &GameStateManager::FocusUnfreeze);
            ForLease->Dispatcher.Attach(NULL, this, WindowEvent::Minimized, &GameStateManager::UnfocusFreeze);
            //ForLease->Dispatcher.Attach(NULL, this, WindowEvent::Maximized, &GameStateManager::FocusUnfreeze);
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
                        if (!frozenLastFrame) {
                            Parent.FrameRateController().Start();   // Start the current frame
                            States[StateIndex]->Update();       // Do all the game stuff
                            Parent.FrameRateController().End();     // End the current frame
                            //State &levelState = ForLease->GameStateManager().CurrentState();
                            //Entity * mute = levelState.AddEntity("Mute");
                            //mute->AddComponent(new Components::SoundEmitter(*mute));
                            //Components::SoundEmitter * emitter = mute->GetComponent<Components::SoundEmitter>(true);
                            //emitter->Rock();
                            ForLease->sound->ResumeAll();
                            ForLease->sound->Update(0.0f);


                        }
                        else {
                            frozenLastFrame = false;
                        }

                        while (Action == StateAction::Freeze) {
                            frozenLastFrame = true;
                            Parent.FrameRateController().SleepFor(0.1);
                            ForLease->OSInput.ProcessAllInput();
//                            State &levelState = ForLease->GameStateManager().CurrentState();
//                            Entity * mute = levelState.AddEntity("Mute");
//                            mute->AddComponent(new Components::SoundEmitter(*mute));
//                            Components::SoundEmitter * emitter = mute->GetComponent<Components::SoundEmitter>(true);
//                            emitter->BeQuiet();
                            ForLease->sound->PauseAll();
                            ForLease->sound->Update(0.0f);  }



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
                                    State &levelState = ForLease->GameStateManager().CurrentState();
                                    Entity * mute = levelState.AddEntity("Mute");
                                    mute->AddComponent(new Components::SoundEmitter(*mute));
                                    Components::SoundEmitter * emitter = mute->GetComponent<Components::SoundEmitter>();
                                    emitter->BeQuiet();
                                }
                                if(frozenLastFrame && Action != StateAction::Freeze) {
                                    Parent.FrameRateController().Start();
                                    Parent.FrameRateController().End();
                                    State &levelState = ForLease->GameStateManager().CurrentState();
                                    Entity * mute = levelState.AddEntity("Mute");
                                    mute->AddComponent(new Components::SoundEmitter(*mute));
                                    Components::SoundEmitter * emitter = mute->GetComponent<Components::SoundEmitter>();
                                    emitter->Rock();
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
        void GameStateManager::SetAction(StateAction action) {
            if(action == StateAction::Freeze && FreezeCount == 0) {
                UnfreezeAction = Action;
                Action = action;
                ++FreezeCount;
            }
            else {
                Action = action;
                FreezeCount = 0;
            }
        }

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
            if(FreezeCount == 0) {
                UnfreezeAction = Action;
                Action = StateAction::Freeze;
            }
            if(FreezeCount < 0) {
                throw(Exception("FreezeCount < 0"));
            }
            ++FreezeCount;
            std::cout << "FreezeCount = " << FreezeCount << std::endl;
        }

        void GameStateManager::FocusUnfreeze(const Event* e) {
            if(FreezeCount > 0) {
                --FreezeCount;
                std::cout << "FreezeCount = " << FreezeCount << std::endl;
                if(FreezeCount == 0) {
                    std::cout << "Unfreezing" << std::endl;
                    Action = UnfreezeAction;
                    Parent.FrameRateController().Start();
                    Parent.FrameRateController().End();
                }
            }
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

        StateAction GameStateManager::GetCurrentAction() const {
            return Action;
        }

    }

} // ForLeaseEngine

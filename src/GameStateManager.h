/*!
    \file   GameStateManager.h
    \author Sean McGeer
    \date   9/14/15
    \brief
        Defines the GameStateManager class.
    \see GameStateManager.cpp

    \copyright ęCopyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

#include <vector>
#include <string>
#include "Event.h"

namespace ForLeaseEngine {

    class Engine;
    class State;

    namespace Modules {

        /*!
            \enum StateAction
            \brief
                The actions to take on the GameStateManager.  Used to switch
                between states or even quit the game.
        */
        enum class StateAction {
            Quit,
            Continue,
            Restart,
            Next,
            Skip,
            Pause,
            Freeze
        };

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
                void Initialize();
                void Run();
                void SetAction(StateAction action);
                void SetState(std::string stateName);
                void SetState(unsigned stateIndex);
                void UnfocusFreeze(const Event* e);
                void FocusUnfreeze(const Event* e);
                State& CurrentState();
                unsigned NumLevels();
                StateAction GetCurrentAction() const;
            private:
                State* StateCurrentlyExecuting;
                //! A reference to the engine
                Engine& Parent;
                //! The vector of all possible states
                std::vector<State *> States;
                //! Index of the current state in States
                unsigned StateIndex;
                //! Index of the next state in States
                unsigned NextStateIndex;
                //! The current action of the GameStateManager
                StateAction Action;

                StateAction UnfreezeAction;

                State* PauseScreen;

                int FreezeCount;

                //! Made private and deactivated, since we NEED Parent
                GameStateManager() = delete;
        };
    }

} // ForLeaseEngine

#endif

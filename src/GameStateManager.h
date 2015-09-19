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

namespace ForLeaseEngine {

    /*!
        \class GameStateManager
        \brief
            A state manager to switch between game states (levels, main menu, and so forth).
    */
    class GameStateManager {

        public:
            GameStateManager();
            void Update();
        private:
            //! The previous state
            STATE_LIST Previous;
            //! The current state
            STATE_LIST Current;
            //! The next state
            STATE_LIST Next;

    };

} // ForLeaseEngine

#endif

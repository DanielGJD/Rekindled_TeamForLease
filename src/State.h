/*!
    \file   State.h
    \author Sean McGeer
    \date   9/17/15
    \brief
        Defines the State class.
    \see State.cpp
*/

#ifndef STATE_H
#define STATE_H

#include <vector>
#include <string>
#include "Entity.h"
#include "StateExceptions.h"

namespace ForLeaseEngine {

    /*!
        \class State
        \brief
            A small class that defines a state to be loaded by the GameStateManager.
    */
    class State {

        public:
            State();
            virtual void Update() = 0;
            Entity* AddEntity();
            Entity* GetEntityByID(long unsigned id, bool throwOnFail = false);
        private:
            std::vector<Entity*> Entities;

    };

} // ForLeaseEngine

#endif

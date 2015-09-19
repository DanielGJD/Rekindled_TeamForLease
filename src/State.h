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
#include "Entity.h"

namespace ForLeaseEngine {

    /*!
        \class State
        \brief
            A small class that defines a state to be loaded by the GameStateManager.
    */
    class State {

        public:
            State() : Entities();
            virtual void Update() = 0;
        private:
            std::vector<Entity *> Entities;

    };

} // ForLeaseEngine

#endif
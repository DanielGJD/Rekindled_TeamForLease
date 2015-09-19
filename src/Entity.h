/*!
    \file   Entity.h
    \author Sean McGeer
    \date   9/17/15
    \brief
        Defines the Entity class.
    \see Entity.cpp
*/

#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include "Component.h"

namespace ForLeaseEngine {

    class Entity {

        public:
            Entity();
            ~Entity();
            long unsigned GetID();
            void AddComponent(Component* component);
        private:
            ComponentID ComponentMask;
            std::vector<Component *> Components;
            long unsigned ID;
            static long unsigned TotalEntities;

    };

} // ForLeaseEngine

#endif

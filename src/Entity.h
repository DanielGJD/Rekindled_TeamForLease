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
#include "ComponentsInclude.h"
#include "EntityExceptions.h"

namespace ForLeaseEngine {

    /*!
        \class Entity
        \brief
            The base class for all game objects.
    */
    class Entity {

        public:
            Entity();
            ~Entity();
            long unsigned GetID();
            void AddComponent(Component* component);
            ComponentType GetComponentMask();
            Component* GetComponent(ComponentType type, bool throwOnFail = false);
        private:
            //! The ID of this Entity, to differentiate from other Entities.
            long unsigned ID;
            //! The mask of all components ORd together that this Entity has.
            ComponentType ComponentMask;
            //! The std::vector of pointers to all Components on this Entity.
            std::vector<Component *> Components;
            //! A static ID of all Entities that have been created by the game.
            static long unsigned TotalEntities;

    };

    void AddComponentsToEntity(ComponentType mask, Entity* entity);

} // ForLeaseEngine

#endif

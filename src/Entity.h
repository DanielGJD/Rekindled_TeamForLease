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
#include "Exception.h"

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
        private:
            //! The mask of all components ORd together that this Entity has.
            ComponentID ComponentMask;
            //! The std::vector of pointers to all Components on this Entity.
            std::vector<Component *> Components;
            //! The ID of this Entity, to differentiate from other Entities.
            long unsigned ID;
            //! A static ID of all Entities that have been created by the game.
            static long unsigned TotalEntities;

    };

    void AddComponentsToEntity(ComponentID mask, Entity* entity);

    /*!
        \class AddComponentException
        \brief
            A customized exception for when adding a component to an entity fails.
    */
    class AddComponentException : public Exception {

        public:
            AddComponentException();
            AddComponentException(const ComponentID id);
            AddComponentException(const ComponentID id, const std::string& message);
            ComponentID GetID();
        private:
            ComponentID ID; //! The type of component that caused the exception.

    };


} // ForLeaseEngine

#endif

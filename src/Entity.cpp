/*!
    \file   Entity.cpp
    \author Sean McGeer
    \date   9/18/15
    \brief
        Implements the Entity class defined in Entity.h.
    \see Entity.h
*/

#include "Entity.h"

namespace ForLeaseEngine {

    long unsigned Entity::TotalEntities = 1;

    /*!
        Constructor for a new Entity.  Sets the ID of this Entity to a new,
        unique ID that is derived from the total number of entities created before
        it plus one.  It also initializes the bitfield mask of components added
        to this entity to cidNone.
    */
    Entity::Entity() : ID(++TotalEntities), ComponentMask(ComponentID::cidNone) {}

    /*!
        Destructor for an Entity.  Destroys all components attached to it.
    */
    Entity::~Entity() {
        for (std::vector<Component *>::iterator it = Components.begin();
                it != Components.end(); ++it)
            delete *it;
    }

    /*!
        Returns the ID of this Entity.

        \return
            A long unsigned integer that is this Entity's ID.
    */
    long unsigned Entity::GetID() {
        return ID;
    }

    /*!
        Add a Component to this Entity.

        \param component
            A pointer to the component to add.
    */
    void Entity::AddComponent(Component* component) {
        ComponentMask |= component->GetID();
        Components.push_back(component);
    }

    /*!
        Helper function to aid in adding multiple Components to an Entity at the
        same time.

        \param mask
            The IDs of all of the components to add to the Entity, OR'd together.

        \param entity
            A pointer to the entity to add the Components to.
    */
    void AddComponentsToEntity(ComponentID mask, Entity* entity) {
        if (!static_cast<bool>(mask))
            throw AddComponentException("No component provided.", mask);

        if (static_cast<bool>(mask & ComponentID::cidTransform))
            entity->AddComponent(new Components::Transform());
        else
            throw AddComponentException("Unimplemented component.", mask);
    }

    /*!
        Constructor for an AddComponentException.  Sets the message to a generic
        error message.
    */
    AddComponentException::AddComponentException()
        : Exception("Error adding component."), ID(ComponentID::cidNone) {}

    /*!
        Constructor for an AddComponentException.  Sets the message to a generic
        error message.

        \param id
            A ComponentID denoting which type of component caused the error.
    */
    AddComponentException::AddComponentException(const ComponentID id)
        : Exception("Error adding component."), ID(id) {}

    /*!
        Constructor for an AddComponentException.

        \param message
            A std::string that is the message to set for this exception.

        \param id
            A ComponentID to determine which type of component caused this error.
    */
    AddComponentException::AddComponentException(const ComponentID id,
        const std::string& message) : Exception(message), ID(id) {}

    /*!
        Gets the type of the component that caused this error.
    */
    ComponentID AddComponentException::GetID() { return ID; }

} // ForLeaseEngine

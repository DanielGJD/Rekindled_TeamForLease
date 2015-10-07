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

    long unsigned Entity::TotalEntities = 0;

    /*!
        Constructor for a new Entity.  Sets the ID of this Entity to a new,
        unique ID that is derived from the total number of entities created before
        it plus one.  It also initializes the bitfield mask of components added
        to this entity to None.
    */
    Entity::Entity() : ID(++TotalEntities), ComponentMask(ComponentType::None) {}

    /*!
        Destructor for an Entity.  Destroys all components attached to it.
    */
    Entity::~Entity() {
        for (Component* component : Components)
            delete component;
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
        if (static_cast<bool>(ComponentMask & component->GetType())) {
            delete component;
            return;
        }
        if ((ComponentMask & component->GetRequired()) != component->GetRequired()) {
            delete component;
            return;
        }

        ComponentMask |= component->GetType();
        Components.push_back(component);
    }

    /*!
        Get the component amsk from this Entity.

        \return
            A ComponentType that is this Entity's ComponentMask
    */
    ComponentType Entity::GetComponentMask() { return ComponentMask; }

    /*!
        Get a Component from this Entity.

        \param type
            The type of the given component.

        \param throwOnFail
            A boolean determining whether to throw an error on failure, rather
            than the default behavior, which is to return a null pointer.
    */
    template <typename T>
    T Entity::GetComponent(ComponentType type, bool throwOnFail) {
        if (type == ComponentType::None && throwOnFail)
            throw EntityException(ID, "No component specified.");

        for (Component* component : Components)
            if (component->GetType() == type)
                return reinterpret_cast<T>(component);

        if (throwOnFail) throw EntityException(ID, "Error finding component.");

        return 0;
    }

    /*!
        Function that checks to see if the Entity has a Component or Components
        registered with it.

        \param type
            A ComponentType that we want to check for.

        \return
            A boolean value--true if the Entity has the Component, false otherwise.
    */
    bool Entity::HasComponent(ComponentType type) {
        if ((type & ComponentMask) == type) return true;
        else return false;
    }

    /*!
        Helper function to aid in adding multiple Components to an Entity at the
        same time.

        \param mask
            The types of all of the components to add to the Entity, OR'd together.

        \param entity
            A pointer to the entity to add the Components to.
    */
    void AddComponentsToEntity(ComponentType mask, Entity* entity) {
        if (!static_cast<bool>(mask))
            throw AddComponentException(mask, entity->GetID(), "No component provided.");

        if (static_cast<bool>(mask & ComponentType::Transform))
            entity->AddComponent(new Components::Transform(*entity));
        if (static_cast<bool>(mask & ComponentType::Physics))
            entity->AddComponent(new Components::Physics(*entity));
        if (static_cast<bool>(mask & ComponentType::Collision))
            entity->AddComponent(new Components::Collision(*entity));
        else
            throw AddComponentException(mask, entity->GetID(), "Unimplemented component.");
    }

} // ForLeaseEngine

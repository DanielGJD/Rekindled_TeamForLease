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

    void Entity::Serialize(Serializer& root) {
        Serializer entity = root.GetChild("Entity");
        entity.WriteUint("ID", static_cast<unsigned>(ID));
        for(unsigned int i = 0; i < Components.size(); ++i) {
            Components[i]->Serialize(entity);
        }
        root.Append(entity, "Entity");
    }

    void Entity::Deserialize(Serializer& root) {
        Serializer entity = root.GetChild("Entity");
        unsigned id;
        entity.ReadUint("ID", id);
        ID = id;
        // for (std::string componentName : entity.GetMemberNames()) {
        //     Serializer componentSerializer = entity.GetChild(componentName);
        //     Component* component;
        // }
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

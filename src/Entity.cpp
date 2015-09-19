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

    Entity::Entity() : ID(++TotalEntities), ComponentMask(ComponentID::cidNone) {}

    long unsigned Entity::GetID() {
        return ID;
    }

    void Entity::AddComponent(Component* component) {
        ComponentMask = static_cast<ComponentID>(static_cast<unsigned long>(component->GetID()) | static_cast<unsigned long>(ComponentMask));
        Components.push_back(component);
    }


    void AddComponentToEntity(ComponentID id, Entity* entity)
    {
        Component* component = new Component(id);

        switch (id) {
            default:
                break;
        }

        entity->AddComponent(component);
    }
} // ForLeaseEngine
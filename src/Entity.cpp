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
    Entity::~Entity() {
        for (std::vector<Component *>::iterator it = Components.begin();
                it != Components.end(); ++it)
            delete *it;
    }

    long unsigned Entity::GetID() {
        return ID;
    }

    void Entity::AddComponent(Component* component) {
        ComponentMask |= component->GetID();
        Components.push_back(component);
    }


    void AddComponentToEntity(ComponentID id, Entity* entity)
    {
        Component* component;

        switch (id) {
            default:
                component = new Component(id);
                break;
        }

        entity->AddComponent(component);
    }
} // ForLeaseEngine

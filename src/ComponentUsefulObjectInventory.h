/*!
    \file   ComponentUsefulObjectInventory.h
    \author Sean McGeer
    \date   3/29/16
    \brief
        Defines the UsefulObjectInventory component.
    \see ComponentUsefulObjectInventory.cpp

    \copyright ©Copyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef COMPONENT_USEFUL_OBJECT_INVENTORY_H
#define COMPONENT_USEFUL_OBJECT_INVENTORY_H

#include "Component.h"
#include "ComponentUsefulObject.h"
#include "CollisionEvent.h"

namespace ForLeaseEngine {

    class Entity;

    namespace Components {

        class UsefulObjectInventory : public Component {

        public:
            static UsefulObjectInventory* Create(Entity& owner);

            static const ComponentType Type = ComponentType::UsefulObjectInventory;
            virtual ComponentType GetType() { return Type; }

            UsefulObjectInventory(Entity& owner);
            ~UsefulObjectInventory();
            void Initialize();

            void Update();
            void OnCollide(const Event* e);

            void Serialize(Serializer& root);
            void Deserialize(Serializer& root);

            UsefulObjectCategory Category;
            std::string FollowName;

        private:
            UsefulObjectInventory() = delete;
        };

    } // Components

} // ForLeaseEngine

#endif // COMPONENT_USEFUL_OBJECT_INVENTORY_H

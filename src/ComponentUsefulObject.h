/*!
    \file   ComponentUsefulObject.h
    \author Sean McGeer
    \date   3/29/16
    \brief
        Defines the UsefulObject component.
    \see ComponentUsefulObject.cpp

    \copyright ęCopyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef COMPONENT_USEFUL_OBJECT_H
#define COMPONENT_USEFUL_OBJECT_H

#include "Component.h"
#include "Event.h"

namespace ForLeaseEngine {

    class Entity;

    enum class UsefulObjectCategory : unsigned char {
        None,
        Balloon,
        Distraction
    };

    namespace Components {

        class UsefulObject : public Component {

            public:
                static UsefulObject* Create(Entity& owner);

                static const ComponentType Type = ComponentType::UsefulObject;
                virtual ComponentType GetType() { return Type; }

                UsefulObject(Entity& owner, UsefulObjectCategory category = UsefulObjectCategory::None);
                ~UsefulObject();
                void Initialize();

                void Update();
                void OnCollide(const Event* e);
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

                UsefulObjectCategory Category;

            private:
                UsefulObject() = delete;

        };

    } // Components

} // ForLeaseEngine

#endif // COMPONENT_USEFUL_OBJECT_H

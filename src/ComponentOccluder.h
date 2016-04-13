/*!
    \file   ComponentOccluder.h
    \author Christopher Hudson

    \brief
        Defines a component that causes an entity to block light/sight

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef COMPONENT_OCCLUDER_H
#define COMPONENT_OCCLUDER_H

#include "Component.h"

namespace ForLeaseEngine {
    namespace Components {
        class Occluder : public Component {
            public:
                static const ComponentType Type = ComponentType::Occluder;
                virtual ComponentType GetType() { return Type; }

                bool BlocksVision;
                bool BlocksLight;

                Occluder(Entity& parent, bool blocksVision = true, bool blocksLight = true);
                ~Occluder();
                void Update();
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);
        };
    }
}

#endif // COMPONENT_OCCLUDER_H

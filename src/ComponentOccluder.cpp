/*!
    \file   ComponentOccluder.cpp
    \author Christopher Hudson

    \brief
        Defines a component that causes an entity to block light/sight

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ComponentOccluder.h"

namespace ForLeaseEngine {
    namespace Components {
        Occluder::Occluder(Entity& parent, bool blocksVision, bool blocksLight) :
                          Component(parent, ComponentType::Transform | ComponentType::Collision),
                          BlocksVision(blocksVision), BlocksLight(blocksLight) {}

        Occluder::~Occluder() {}

        void Occluder::Update() {}

        void Occluder::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned int>(Type));
            Serializer occluder = root.GetChild("Occluder");
            occluder.WriteBool("BlocksVision", BlocksVision);
            occluder.WriteBool("BlocksLight", BlocksLight);
            root.Append(occluder, "Occluder");
        }

        void Occluder::Deserialize(Serializer& root) {
            Serializer occluder = root.GetChild("Occluder");
            occluder.ReadBool("BlocksVision", BlocksVision);
            occluder.ReadBool("BlocksLight", BlocksLight);
        }
    }
}

/*!
    \file   LevelComponentLight.cpp
    \author Christopher Hudson

    \brief
        Implements the Light level component.

    \see    LevelComponentLight.h

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "LevelComponentLight.h"
#include "Engine.h"
#include "LevelComponentRenderer.h"
#include "Ray.h"

namespace ForLeaseEngine {

    namespace LevelComponents {

        Light::Light(State& owner) : LevelComponent(owner) { }

        void Light::Update(std::vector<Entity *>& entities) {
        }

        void Light::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(ComponentType::Light));
            Serializer light = root.GetChild("Light");
            light.WriteUint("Type", static_cast<unsigned>(ComponentType::Light));
            root.Append(light, "Light");
        }

        void Light::Deserialize(Serializer& root) {
            Serializer light = root.GetChild("Light");
        }

    }

}

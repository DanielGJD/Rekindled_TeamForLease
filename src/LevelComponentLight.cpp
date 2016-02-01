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
            LitEntities.clear();

            for(unsigned int i = 0; i < entities.size(); ++i) {
                if(entities[i]->HasComponent(ComponentType::Light)) {
                    Components::Light* light = entities[i]->GetComponent<Components::Light>();
                    if(light->Active) {
                        std::unordered_set<unsigned long> lit = light->ComputeLighting();

                        for(std::unordered_set<unsigned long>::const_iterator i = lit.begin(); i != lit.end(); ++i) {
                            LitEntities.insert(*i);
                        }
                    }
                }
            }
        }

        void Light::Serialize(Serializer& root) {
            Serializer light = root.GetChild("Light");
            light.WriteUint("Type", static_cast<unsigned>(ComponentType::Light));
            root.Append(light, "Light");
        }

        void Light::Deserialize(Serializer& root) {
            Serializer light = root.GetChild("Light");
        }

        bool Light::CheckIfLit(unsigned long id) {
            if(LitEntities.find(id) != LitEntities.end())
                return true;

            return false;
        }
    }

}

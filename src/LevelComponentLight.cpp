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

        Light::Light(State& owner, Color const& ambientLight) : LevelComponent(owner), AmbientLight(ambientLight) { }

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
            root.WriteUint("Type", static_cast<unsigned>(ComponentType::Light));
            Serializer light = root.GetChild("Light");
            light.WriteUint("Type", static_cast<unsigned>(ComponentType::Light));
            Serializer ambient = light.GetChild("AmbientLight");
            AmbientLight.Serialize(ambient);
            light.Append(ambient, "AmbientLight");
            root.Append(light, "Light");
        }

        void Light::Deserialize(Serializer& root) {
            Serializer light = root.GetChild("Light");
            Serializer ambient = light.GetChild("AmbientLight");
            AmbientLight.Deserialize(ambient);
        }

        bool Light::CheckIfLit(unsigned long id) {
            if(LitEntities.find(id) != LitEntities.end())
                return true;

            return false;
        }
    }

}

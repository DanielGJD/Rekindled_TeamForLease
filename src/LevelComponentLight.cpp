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
            /*for (Entity* entity : entities) {
                if (!entity->HasComponent(ComponentType::Light)) continue;

                Components::Light* light = entity->GetComponent<Components::Light>(true);

                // Raycasting stuff
                Vector start = Vector::Rotate(light->Direction, -light->Sweep / 2);

                float rotStep = light->Sweep / light->Rays;

                Vector rayVec = start;

                LevelComponents::Renderer* renderer = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>();


                renderer->SetDrawingColor(light->DrawColor);

                // The Andrew Method
                for (unsigned i = 0; i <= light->Rays; ++i) {
                    Ray ray(entity->GetComponent<Components::Transform>()->Position, rayVec, 666);
                    for (Entity* toCheck : entities) {
                        if (!toCheck->HasComponent(ComponentType::Collision)) continue;

                        ray.IsColliding(toCheck);
                    }

                    renderer->DrawLine(ray.GetStart(), ray.GetScaledVector());


                    rayVec = Vector::Rotate(rayVec, rotStep);
                }
            }*/
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

/*!
    \file   ComponentLight.cpp
    \author Sean McGeer
    \date   10/30/15
    \brief
        Implements the Light component.
    \see ComponentLight.h

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ComponentLight.h"
#include "ComponentCollision.h"
#include "Entity.h"
#include "Ray.h"
#include "State.h"
#include <cmath>

namespace ForLeaseEngine {

    namespace Components {

        Light::Light(Entity& owner, float sweep, unsigned rays)
            : Component(owner, ComponentType::Transform), Rays(rays) {}

        void Light::Update() {
            // Raycasting stuff

            float rotation = Parent.GetComponent<Components::Transform>()->Rotation;
            Vector mid = Vector::Rotate(Vector(1,0), rotation);
            Vector start = Vector::Rotate(mid, -Sweep / 2);

            float rotStep = Sweep / Rays;

            Vector rayVec = start;

            LevelComponents::Renderer* renderer = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>();


            renderer->SetDrawingColor(1, 1, 153.0f/255, 1);

            for (unsigned i = 0; i <= Rays; ++i) {
                Ray ray(Parent.GetComponent<Components::Transform>()->Position, rayVec, Ray::Unlimited);
                std::vector<Entity *> entities = ForLease->GameStateManager().CurrentState().GetAllEntities();
                for (Entity* entity : entities) {
                    if (!entity->HasComponent(ComponentType::Collision)) continue;

                    ray.IsColliding(entity);
                }

                renderer->DrawLine(ray.GetStart(), ray.GetScaledVector());


                rayVec = Vector::Rotate(rayVec, rotStep);
            }

        }

        void Light::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(Type));
            Serializer light = root.GetChild("Light");
            light.WriteVec("Direction", Direction);
            light.WriteFloat("Sweep", Sweep);
            light.WriteUint("Rays", Rays);
            light.WriteUint("Type", static_cast<unsigned>(Type));
            root.Append(light, "Light");
        }

        void Light::Deserialize(Serializer& root) {
            Serializer light = root.GetChild("Light");
            light.ReadVec("Direction", Direction);
            light.ReadFloat("Sweep", Sweep);
            light.ReadUint("Rays", Rays);
        }

    } // Components

} // ForLeaseEngine

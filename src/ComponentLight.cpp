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

        Light::Light(Entity& owner, Vector direction, Color drawColor, float sweep, unsigned rays)
            : Component(owner, ComponentType::Transform), Direction(direction), DrawColor(drawColor), Sweep(sweep), Rays(rays) {}

        void Light::Update() {
            // Raycasting stuff
            Vector start = Vector::Rotate(Direction, -Sweep / 2);

            float rotStep = Sweep / Rays;

            Vector rayVec = start;

            LevelComponents::Renderer* renderer = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>();


            renderer->SetDrawingColor(DrawColor);

            // The Andrew Method
            for (unsigned i = 0; i <= Rays; ++i) {
                Ray ray(Parent.GetComponent<Components::Transform>()->Position, rayVec, 666);
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
            DrawColor.Serialize(light);
            root.Append(light, "Light");
        }

        void Light::Deserialize(Serializer& root) {
            Serializer light = root.GetChild("Light");
            light.ReadVec("Direction", Direction);
            light.ReadFloat("Sweep", Sweep);
            light.ReadUint("Rays", Rays);
            DrawColor.Deserialize(light);
        }

    } // Components

} // ForLeaseEngine

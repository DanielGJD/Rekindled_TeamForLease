/*!
    \file   ComponentLight.cpp
    \author Sean McGeer
    \date   10/30/15
    \brief
        Implements the Light component.
    \see ComponentLight.h
*/

#include "ComponentLight.h"
#include "ComponentCollision.h"
#include "Entity.h"
#include "Ray.h"
#include "State.h"
#include <cmath>

namespace ForLeaseEngine {

    namespace Components {

        Light::Light(Entity& owner, Vector start, Vector end,
            unsigned additional, float length)
            : Component(owner, ComponentType::Transform), Start(start), End(end),
              Length(length), Additional(additional) {}

        void Light::Update() {
            // Raycasting stuff

            float cosTheta = Vector::DotProduct(Start, End);
            cosTheta /= Start.Magnitude() * End.Magnitude();

            float theta = acos(cosTheta);

            float rotStep = theta/Additional;

            Vector rayVec = Start;

            LevelComponents::Renderer* renderer = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>();


            renderer->SetDrawingColor(1, 1, 153.0f/255, 1);

            for (unsigned i = 0; i <= Additional; ++i) {
                Ray ray(Parent.GetComponent<Components::Transform>()->Position, rayVec, Length);
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
            light.WriteVec("Start", Start);
            light.WriteVec("End", End);
            light.WriteFloat("Length", Length);
            light.WriteUint("Additional", Additional);
            light.WriteUint("Type", static_cast<unsigned>(Type));
            root.Append(light, "Light");
        }

        void Light::Deserialize(Serializer& root) {
            Serializer light = root.GetChild("Light");
            light.ReadVec("Start", Start);
            light.ReadVec("End", End);
            light.ReadFloat("Length", Length);
            light.ReadUint("Additional", Additional);
        }

    } // Components

} // ForLeaseEngine

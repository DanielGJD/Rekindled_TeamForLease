/*!
    \file   SystemPhysics.cpp
    \author Sean McGeer
    \date   9/24/15
    \brief
        Implements the Physics system.
    \see SystemPhysics.h
*/

namespace ForLeaseEngine {

    namespace Systems {

        Physics::Physics(Engine& owner) : System(owner, ComponentType::Physics) {}

        void Physics::Update(std::vector<Entity *> entities) {

            for (Entity* entity : entities) {
                if (CheckEntityCompatibility(entity)) {
                    ApplyGravity();
                }
            }

        }

        void ApplyGravity(Entity* entity) {
            Components::Physics* physicsComponent = entity->GetComponent(ComponentType::Physics);
            Components::Transform* transformComponent = entity->GetComponent(ComponentType::Transform);

            physicsComponent->Acceleration = Gravity;
            physicsComponent->Velocity = physicsComponent->Velocity + Acceleration * ForLease->FrameRateController().GetDt();
            transformComponent->Position = transformComponent->Position + physicsComponent->Velocity * ForLease->FrameRateController()>GetDt();

        }

        void Physics::SetGravity(Vector gravity) { Gravity = gravity };

        Vector Physics::GetGravity() { return Gravity; }

    }

}

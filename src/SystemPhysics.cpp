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

        void Physics::Update(std::vector<Entity *>& entities) {

            for (Entity* entity : entities) {
                if (CheckEntityCompatibility(entity)) {
                    ApplyGravity(entity);
                    Compute(entity);
                    Cleanup(entity);
                }
            }

        }
        
        void Physics::Compute(Entity* entity) {
            Components::Physics* physicsComponent = entity->GetComponent(ComponentType::Physics);
            Components::Transform* transformComponent = entity->GetComponent(ComponentType::Transform);
        
            physicsComponent->Velocity += Acceleration * ForLease->FrameRateController().GetDt();
            transformComponent->Position += physicsComponent->Velocity * ForLease->FrameRateController().GetDt();
        }
        
        void Physics::Cleanup(Entity* entity) {
            entity->Acceleration[0] = 0;
            entity->Acceleration[1] = 0;
            entity->Force[0] = 0;
            entity->Force[1] = 0;
        }

        void Physics::ApplyGravity(Entity* entity) {
            Components::Physics* physicsComponent = entity->GetComponent(ComponentType::Physics);

            physicsComponent->Acceleration += Gravity;
        }

        void Physics::SetGravity(Vector gravity) { Gravity = gravity };

        Vector Physics::GetGravity() { return Gravity; }

    }

}

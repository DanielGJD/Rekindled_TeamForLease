/*!
    \file   LevelComponentPhysics.cpp
    \author Sean McGeer
    \date   9/24/15
    \brief
        Implements the Physics system.
    \see LevelComponentPhysics.h

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "LevelComponentPhysics.h"
#include <iostream>

namespace ForLeaseEngine {

    namespace LevelComponents {

        /*!
            Constructor for the new Physics level component.

            \param owner
                A reference to the State that created it.

            \param gravity
                A vector to use as the gravity to be applied each frame.
        */
        Physics::Physics(State& owner, Vector gravity)
            : LevelComponent(owner, ComponentType::Physics), Gravity(gravity) {}

        /*!
            Update function.  Calls ApplyGravity(), Compute(), and Cleanup()
            on each Entity.

            \param entities
                A reference to a vector of pointers to Entities that we want
                to process.
        */
        void Physics::Update(std::vector<Entity *>& entities) {

            //std::cout << Gravity << std::endl;

            for (Entity* entity : entities) {
                if (CheckEntityCompatibility(entity)) {
                    ApplyGravity(entity);
                    Compute(entity);
                    Cleanup(entity);
                }
            }

        }

        /*!
            Applies the Gravity vector to an entity.  Simply adds the Gravity
            vector to the Acceleration vector.
        */
        void Physics::ApplyGravity(Entity* entity) {
            Components::Physics* physicsComponent = entity->GetComponent<Components::Physics>();

            physicsComponent->Acceleration += Gravity * physicsComponent->Mass;
        }

        /*!
            Computes the change in position, velocity, and acceleration for this
            frame.

            \param entity
                A pointer to the entity to compute physics for.
        */
        void Physics::Compute(Entity* entity) {
            Components::Physics* physicsComponent = entity->GetComponent<Components::Physics>();
            Components::Transform* transformComponent = entity->GetComponent<Components::Transform>();

            float dt;

            if (physicsComponent->UnaffectedByTimeScaling) // This moves the same amount, no matter what time scaling is at
                dt = ForLease->FrameRateController().GetUnscaledDt();
            else                                           // This slows down as the world does
                dt = ForLease->FrameRateController().GetDt();

            physicsComponent->Velocity += physicsComponent->Acceleration * dt;
            transformComponent->Position += physicsComponent->Velocity * dt;
        }

        /*!
            Cleans up the acceleration and forces for an entity.  We want
            instantaneous acceleration/velocity, so we zero those out.

            \param entity
                A pointer to the entity to clean up.
        */
        void Physics::Cleanup(Entity* entity) {
            Components::Physics* physicsComponent = entity->GetComponent<Components::Physics>();
            physicsComponent->Acceleration[0] = 0;
            physicsComponent->Acceleration[1] = 0;
            physicsComponent->Force[0] = 0;
            physicsComponent->Force[1] = 0;
        }

        /*!
            Set the gravity to use for each Entity passed to the LevelComponent.

            \param gravity
                The vector we want to use as the new gravity.
        */
        void Physics::SetGravity(Vector gravity) { Gravity = gravity; };

        /*!
            Get the current gravity.

            \return
                The gravity Vector.
        */
        Vector Physics::GetGravity() { return Gravity; }

        void Physics::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(ComponentType::Physics));
            Serializer physics = root.GetChild("Physics");
            physics.WriteVec("Gravity", Gravity);
            physics.WriteUint("Type", static_cast<unsigned>(ComponentType::Physics));
            root.Append(physics, "Physics");
        }

        void Physics::Deserialize(Serializer& root) {
            Serializer physics = root.GetChild("Physics");
            physics.ReadVec("Gravity", Gravity);
        }

    } // LevelComponents

} // ForLeaseEngine

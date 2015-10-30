/*!
    \file   LevelComponentPhysics.h
    \author Sean McGeer
    \date   9/24/15
    \brief
        Defines the Physics system.
    \see LevelComponentPhysics.cpp
*/

#ifndef LC_PHYSICS_H
#define LC_PHYSICS_H

#include "LevelComponent.h"
#include "ComponentPhysics.h"
#include "ComponentTransform.h"
#include "Vector.h"

namespace ForLeaseEngine {

    namespace LevelComponents {

        /*!
            \class Physics

            \brief
                A LevelComponent that computes changes in position, velocity,
                and acceleration.  Also applies gravity to each Entity with
                a physics component.
        */
        class Physics : public LevelComponent {
            public:
                static const ComponentType Type = ComponentType::Physics;
                virtual ComponentType GetType() { return Type; }
                Physics(State& owner, Vector gravity = Vector(0, -1));
                void Update(std::vector<Entity *>& entities);
                void ApplyGravity(Entity* entity);
                void Compute(Entity* entity);
                void Cleanup(Entity* entity);
                void SetGravity(Vector gravity);
                Vector GetGravity();

                virtual void Serialize(Serializer& root);
                virtual void Deserialize(Serializer& root);
            private:
                Vector Gravity; //! The level's gravity
        };

    } // LevelComponents

} // ForLeaseEngine

#endif

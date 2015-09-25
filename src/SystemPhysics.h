/*!
    \file   SystemPhysics.h
    \author Sean McGeer
    \date   9/24/15
    \brief
        Defines the Physics system.
    \see SystemPhysics.cpp
*/

#ifndef SYSTEM_PHYSICS_H
#define SYSTEM_PHYSICS_H

#include "System.h"
#include "ComponentPhysics.h"
#include "ComponentTransform.h"
#include "Vector.h"

namespace ForLeaseEngine {

    namespace Systems {

        class Physics : public System {
            public:
                Physics(Engine& owner, Vector gravity = Vector(0, -1));
                void Update(std::vector<Entity *> entities);
                void ApplyGravity(Entity* entity);
                void SetGravity(Vector gravity);
                Vector GetGravity();
            private:
                Vector Gravity;
        };

    }

} // ForLeaseEngine

#endif

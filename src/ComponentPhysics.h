/*!
    \file   ComponentPhysics.h
    \author Sean McGeer
    \date   9/24/15
    \brief
        Defines the Physics component.
    \see ComponentPhysics.cpp
*/


#ifndef COMPONENT_PHYSICS_H
#define COMPONENT_PHYSICS_H

#include "Component.h"
#include "Vector.h"

namespace ForLeaseEngine {

    class Entity;

    namespace Components {

        class Physics : public Component {
            public:
                Physics(Entity& owner, float mass = 1, Vector velocity = Vector(0,0),
                    Vector acceleration = Vector(0,0), Vector force = Vector(0,0));
                void Update() {}
                float Mass;
                Vector Velocity;
                Vector Acceleration;
                Vector Force;
        };

    } // Components

} // ForLeaseEngine

#endif

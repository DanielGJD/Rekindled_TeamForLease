/*!
    \file   ComponentPhysics.h
    \author Sean McGeer
    \date   9/24/15
    \brief
        Defines the Physics component.
    \see ComponentPhysics.cpp

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/


#ifndef COMPONENT_PHYSICS_H
#define COMPONENT_PHYSICS_H

#include "Component.h"
#include "Vector.h"

namespace ForLeaseEngine {

    class Entity;

    namespace Components {

        /*!
            \class Physics

            \brief
                A basic Physics component that stores the Mass, Velocity, Acceleration,
                and net Force on the owning entity.
        */
        class Physics : public Component {
            public:
                static const ComponentType Type = ComponentType::Physics;
                virtual ComponentType GetType() { return Type; }
                Physics(Entity& owner, float mass = 1, Vector velocity = Vector(0,0),
                    Vector acceleration = Vector(0,0), Vector force = Vector(0,0),
                    bool unaffectedByTimeScaling = false, bool unaffectedByGravity = false);
                void Update() {}
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);
                float  Mass;                    //! The mass of the owning entity.
                Vector Velocity;                //! Current velocity
                Vector Acceleration;            //! The current (instantaneous) acceleration.
                Vector Force;                   //! The net forces on the object.
                bool   UnaffectedByTimeScaling; //! Whether or not this physics component is affected by time scaling.
                bool   UnaffectedByGravity;     //! Whether or not this physics component is affected by gravity.
            private:
                Physics() = delete;
        };

    } // Components

} // ForLeaseEngine

#endif

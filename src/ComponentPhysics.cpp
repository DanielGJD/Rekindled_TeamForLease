/*!
    \file   ComponentPhysics.cpp
    \author Sean McGeer
    \date   9/24/15
    \brief
        Implements the Physics component.
    \see ComponentPhysics.h

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ComponentPhysics.h"
#include "Engine.h"
// #include "Entity.h"

namespace ForLeaseEngine {

    namespace Components {

        /*!
            Constructor for the Physics component.

            \param owner
                The owning Entity.

            \param mass
                A float representing the mass of the object.  Defaults to 1.

            \param velocity
                A vector representing the current velocity of the object.
                Defaults to <0,0>.

            \param acceleration
                A vector represeting the current acceleration on the object.
                Defaults to <0,0>.

            \param force
                A vector representing the current force on the object.
                Defaults to <0,0>.
        */
        Physics::Physics(Entity& owner, float mass, Vector velocity,
            Vector acceleration, Vector force, bool unaffectedByTimeScaling,
            bool unaffectedByGravity) : Component(owner, ComponentType::Transform),
              Mass(mass), Velocity(velocity), Acceleration(acceleration),
              Force(force), UnaffectedByTimeScaling(unaffectedByTimeScaling),
              UnaffectedByGravity(unaffectedByGravity) {}

        Vector Physics::GetFrameVelocity() {
            float dt;

            if (UnaffectedByTimeScaling) // This moves the same amount, no matter what time scaling is at
                dt = ForLease->FrameRateController().GetUnscaledDt();
            else                                           // This slows down as the world does
                dt = ForLease->FrameRateController().GetDt();

            return Velocity * dt;
        }

        void Physics::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(Type));
            Serializer physics = root.GetChild("Physics");
            physics.WriteFloat("Mass", Mass);
            physics.WriteVec("Velocity", Velocity);
            physics.WriteVec("Acceleration", Acceleration);
            physics.WriteVec("Force", Force);
            physics.WriteBool("UnaffectedByTimeScaling", UnaffectedByTimeScaling);
            physics.WriteBool("UnaffectedByGravity", UnaffectedByGravity);
            physics.WriteUint("Type", static_cast<unsigned>(Type));
            root.Append(physics, "Physics");
        }

        void Physics::Deserialize(Serializer& root) {
            Serializer physics = root.GetChild("Physics");
            physics.ReadFloat("Mass", Mass);
            physics.ReadVec("Velocity", Velocity);
            physics.ReadVec("Acceleration", Acceleration);
            physics.ReadVec("Force", Force);
            physics.ReadBool("UnaffectedByTimeScaling", UnaffectedByTimeScaling);
            physics.ReadBool("UnaffectedByGravity", UnaffectedByGravity);
        }
    } // Components

} // ForLeaseEngine

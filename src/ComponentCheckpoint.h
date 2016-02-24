/*!
    \file   ComponentCheckpoint.h
    \author Sean McGeer
    \date   2/24/16
    \brief
        Defines the Checkpoint component.
    \see ComponentCheckpoint.cpp

    \copyright ©Copyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/


#ifndef COMPONENT_CHECKPOINT_H
#define COMPONENT_CHECKPOINT_H

#include "Component.h"
#include "Vector.h"
#include "Event.h"

namespace ForLeaseEngine {

    class Entity;

    namespace Components {

        /*!
            \class Checkpoint

            \brief
                A basic Physics component that stores the Mass, Velocity, Acceleration,
                and net Force on the owning entity.
        */
        class Checkpoint : public Component {
            public:
                static const ComponentType Type = ComponentType::Checkpoint;
                virtual ComponentType GetType() { return Type; }
                Checkpoint(Entity& owner);
                void Update() {}
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);
            private:
                bool Active;
                Checkpoint() = delete;
        };

    } // Components

} // ForLeaseEngine

#endif // COMPONENT_CHECKPOINT_H

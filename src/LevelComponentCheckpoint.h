/*!
    \file   LevelComponentCheckpoint.h
    \author Sean McGeer
    \date   2/24/16
    \brief
        Defines the Checkpoint system.
    \see LevelComponentCheckpoint.cpp

    \copyright ©Copyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef LC_CHECKPOINT_H
#define LC_CHECKPOINT_H

#include "LevelComponent.h"
#include "ComponentCheckpoint.h"
#include "ComponentTransform.h"
#include "Vector.h"
#include "Event.h"

namespace ForLeaseEngine {

    namespace LevelComponents {

        /*!
            \class Checkpoint

            \brief
                A LevelComponent that computes changes in position, velocity,
                and acceleration.  Also applies gravity to each Entity with
                a physics component.
        */
        class Checkpoint : public LevelComponent {
            public:
                static const ComponentType Type = ComponentType::Checkpoint;
                virtual ComponentType GetType() { return Type; }
                Checkpoint(State& owner, Serializer& root);
                ~Checkpoint();
                void Update(std::vector<Entity *>& entities);

                void CheckpointActivated(const Event* e);
                void ResetToCheckpoint();

                virtual void Serialize(Serializer& root);
                virtual void Deserialize(Serializer& root);

                unsigned long TriggerEntityID;
                Serializer LastCheckpointState;

                void OnKeyDown(const Event* e);
            private:
                
        };

    } // LevelComponents

} // ForLeaseEngine

#endif // LC_CHECKPOINT_H

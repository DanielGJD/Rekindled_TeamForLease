/*!
    \file   LevelComponentCheckpoint.cpp
    \author Sean McGeer
    \date   2/24/16
    \brief
        Implements the Checkpoint system.
    \see LevelComponentCheckpoint.h

    \copyright ©Copyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/

#include "LevelComponentCheckpoint.h"
#include "State.h"
#include <iostream>

namespace ForLeaseEngine {

    namespace LevelComponents {

        /*!
            Constructor for the new Checkpoint level component.

            \param owner
                A reference to the State that created it.
        */
        Checkpoint::Checkpoint(State& owner) : LevelComponent(owner, ComponentType::Checkpoint), LastCheckpointState() {
            ForLease->Dispatcher.Attach(NULL, this, "CheckpointActivated", &Checkpoint::CheckpointActivated);
        }

        void Checkpoint::Update(std::vector<Entity *>& entities) {

//            for (Entity* entity : entities) {
//                if (CheckEntityCompatibility(entity)) {
//
//                }
//            }
        }

        void Checkpoint::CheckpointActivated(const Event* e) {
            Owner.Serialize(LastCheckpointState);
        }

        void Checkpoint::ResetToCheckpoint() {
            Owner.Deserialize(LastCheckpointState);
        }

        void Checkpoint::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(ComponentType::Checkpoint));
            Serializer checkpoint = root.GetChild("Checkpoint");
            checkpoint.WriteUint("Type", static_cast<unsigned>(ComponentType::Checkpoint));
            root.Append(checkpoint, "Checkpoint");
        }

        void Checkpoint::Deserialize(Serializer& root) {
            Serializer checkpoint = root.GetChild("Checkpoint");
        }

    } // LevelComponents

} // ForLeaseEngine

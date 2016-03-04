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
        Checkpoint::Checkpoint(State& owner, Serializer& root) : LevelComponent(owner, ComponentType::Checkpoint), LastCheckpointState(root),
            TriggerEntityID(0) {
            ForLease->Dispatcher.Attach(NULL, this, "CheckpointActivated", &Checkpoint::CheckpointActivated);
            ForLease->Dispatcher.Attach(NULL, this, "KeyDown", &Checkpoint::OnKeyDown);
        }

        Checkpoint::~Checkpoint() {
            ForLease->Dispatcher.Detach(this, "KeyDown");
            ForLease->Dispatcher.Detach(this, "CheckpointActivated");
        }

        void Checkpoint::Update(std::vector<Entity *>& entities) {

//            for (Entity* entity : entities) {
//                if (CheckEntityCompatibility(entity)) {
//
//                }
//            }
        }

        void Checkpoint::CheckpointActivated(const Event* e) {
            LastCheckpointState = Serializer();
            Owner.Serialize(LastCheckpointState);
        }

        void Checkpoint::ResetToCheckpoint() {
            //Owner.DeserializeNonReference(LastCheckpointState);
            Owner.ResetToCheckpoint = true;
        }

        void Checkpoint::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(ComponentType::Checkpoint));
            Serializer checkpoint = root.GetChild("Checkpoint");
            checkpoint.WriteUint("TriggerEntityID", static_cast<unsigned>(TriggerEntityID));
            checkpoint.WriteUint("Type", static_cast<unsigned>(ComponentType::Checkpoint));
            root.Append(checkpoint, "Checkpoint");
        }

        void Checkpoint::Deserialize(Serializer& root) {
            Serializer checkpoint = root.GetChild("Checkpoint");
            unsigned triggerID;
            checkpoint.ReadUint("TriggerEntityID", triggerID);
            TriggerEntityID = triggerID;
        }

        void Checkpoint::OnKeyDown(const Event* e) {
            const KeyboardEvent* key_e = static_cast<const KeyboardEvent*>(e);

            if (key_e->Key == Keys::P)
                ResetToCheckpoint();
        }

    } // LevelComponents

} // ForLeaseEngine

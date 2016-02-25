/*!
    \file   ComponentCheckpoint.cpp
    \author Sean McGeer
    \date   2/24/16
    \brief
        Implements the Checkpoint component.
    \see ComponentCheckpoint.h

    \copyright ęCopyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ComponentCheckpoint.h"
#include "LevelComponentCheckpoint.h"
#include "Engine.h"
#include "GameStateManager.h"
#include "State.h"
#include "CollisionEvent.h"
// #include "Entity.h"

namespace ForLeaseEngine {

    namespace Components {

        /*!
            Constructor for the Checkpoint component.

            \param owner
                The owning Entity.
        */
        Checkpoint::Checkpoint(Entity& owner) : Component(owner, ComponentType::Transform | ComponentType::Collision),
            Active(false) {
                ForLease->Dispatcher.Attach(NULL, this, "Collision", &Checkpoint::OnCollide);
            }

        ~Checkpoint::Checkpoint() {
            ForLease->Dispatcher.Detach(this, "Collision");
        }

        void Checkpoint::OnCollide(const CollisionEvent* e) {
            LevelComponents::Checkpoint* lcCheckpoint = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Checkpoint>();
            if (!lcCheckpoint) throw Exception("No checkpoint level component found!");

            if (e->Other->GetID() == lcCheckpoint->TriggerEntityID && !Active) {
                Active = true;
                Event activatedEvent = Event("CheckpointActivated");
                ForLease->Dispatcher.Dispatch(&activatedEvent, this);
            }
        }

        void Checkpoint::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(Type));
            Serializer checkpoint = root.GetChild("Checkpoint");
            checkpoint.WriteBool("Active", Active);
            checkpoint.WriteUint("Type", static_cast<unsigned>(Type));
            root.Append(checkpoint, "Checkpoint");
        }

        void Checkpoint::Deserialize(Serializer& root) {
            Serializer checkpoint = root.GetChild("Checkpoint");
            checkpoint.ReadBool("Active", Active);
        }
    } // Components

} // ForLeaseEngine

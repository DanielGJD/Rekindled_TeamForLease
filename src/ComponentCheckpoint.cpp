/*!
    \file   ComponentCheckpoint.cpp
    \author Sean McGeer
    \date   2/24/16
    \brief
        Implements the Checkpoint component.
    \see ComponentCheckpoint.h

    \copyright ©Copyright 2016 DigiPen Institute of Technology, All Rights Reserved
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

        Checkpoint* Checkpoint::Create(Entity& owner) {
            Checkpoint* checkpoint = new Checkpoint(owner);
            checkpoint->Initialize();
            return checkpoint;
        }

        /*!
            Constructor for the Checkpoint component.

            \param owner
                The owning Entity.
        */
        Checkpoint::Checkpoint(Entity& owner) : Component(owner, ComponentType::Transform | ComponentType::Collision),
            Active(false) {}

        Checkpoint::~Checkpoint() {
            ForLease->Dispatcher.Detach(this, "Collision");
        }

        void Checkpoint::Initialize() {
            ForLease->Dispatcher.Attach(NULL, this, "Collision", &Checkpoint::OnCollide, &Parent);
        }

        void Checkpoint::OnCollide(const Event* e) {
            const CollisionEvent* collision = reinterpret_cast<const CollisionEvent*>(e);

            LevelComponents::Checkpoint* lcCheckpoint = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Checkpoint>();
            if (!lcCheckpoint) throw Exception("No checkpoint level component found!");

            if (collision->With->GetID() == lcCheckpoint->TriggerEntityID && !Active) {
                Active = true;

                Components::Light* light = Parent.GetComponent<Components::Light>();
                Components::ParticleEmitter* pemit = Parent.GetComponent<Components::ParticleEmitter>();

                if (light) {
                    light->Active = true;
                }

                if (pemit) {
                    pemit->Active = true;
                }

                ForLease->sound->PlayEvent(ActivateSound);

                Event activatedEvent = Event("CheckpointActivated");
                ForLease->Dispatcher.Dispatch(&activatedEvent, this);
            }
        }

        void Checkpoint::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(Type));
            Serializer checkpoint = root.GetChild("Checkpoint");
            checkpoint.WriteBool("Active", Active);
            checkpoint.WriteString("ActivateSound", ActivateSound);
            checkpoint.WriteUint("Type", static_cast<unsigned>(Type));
            root.Append(checkpoint, "Checkpoint");
        }

        void Checkpoint::Deserialize(Serializer& root) {
            Serializer checkpoint = root.GetChild("Checkpoint");
            checkpoint.ReadBool("Active", Active);
            checkpoint.ReadString("ActivateSound", ActivateSound);
            if (ActivateSound == "") ActivateSound = "event_checkpoint_ignite01";
        }
    } // Components

} // ForLeaseEngine

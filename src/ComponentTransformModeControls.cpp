#include "ComponentTransformModeControls.h"
#include "Keys.h"
#include "Engine.h"
#include "State.h"
#include "KeyboardEvent.h"

namespace ForLeaseEngine {
    namespace Components {
        TransformModeControls::TransformModeControls(Entity& owner)
                                                    : Component(owner), SlowMotionSpeed(0.5f), NormalSpeed(1),
                                                      ModeToggleKey(Keys::LeftAlt), Active(false), EntitySelected(false), ActiveEntity(0) {}

        TransformModeControls::~TransformModeControls() {
            ForLease->Dispatcher.Detach(this, "KeyUp");
        }

        void TransformModeControls::Initialize() {
            ForLease->Dispatcher.Attach(NULL, this, "KeyUp", &TransformModeControls::OnKeyUp);
        }

        void TransformModeControls::Update() {
        }

        void TransformModeControls::Serialize(Serializer& root) {
        }

        void TransformModeControls::Deserialize(Serializer& root) {
        }

        void TransformModeControls::Activate() {
            Active = true;

            if(EntitySelected) {
                Entity* entity = ForLease->GameStateManager().CurrentState().GetEntityByID(ActiveEntity);

                if(entity) {
                    if(entity->HasComponent(ComponentType::DragWithMouse)) {
                        entity->GetComponent<Components::DragWithMouse>()->Active = true;
                    }
                }
            }

            ForLease->FrameRateController().TimeScaling(SlowMotionSpeed);
        }

        void TransformModeControls::Deactivate() {
            Active = false;

            if(EntitySelected) {
                Entity* entity = ForLease->GameStateManager().CurrentState().GetEntityByID(ActiveEntity);

                if(entity) {
                    if(entity->HasComponent(ComponentType::DragWithMouse)) {
                        entity->GetComponent<Components::DragWithMouse>()->Active = false;
                    }
                }

                EntitySelected = false;
            }

            ForLease->FrameRateController().TimeScaling(NormalSpeed);
        }

        void TransformModeControls::Toggle() {
            if(Active) {
                Deactivate();
            }
            else {
                Activate();
            }
        }

        void TransformModeControls::OnKeyUp(Event const* e) {
            KeyboardEvent const* key_e = static_cast<KeyboardEvent const*>(e);

            if(key_e->Key == Keys::LeftAlt) {
                Toggle();
            }
        }
    }
}

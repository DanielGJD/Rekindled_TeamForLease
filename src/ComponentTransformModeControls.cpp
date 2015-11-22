#include "ComponentTransformModeControls.h"
#include "Keys.h"
#include "Engine.h"
#include "State.h"
#include "KeyboardEvent.h"
#include "MouseButtonEvent.h"

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
            ForLease->Dispatcher.Attach(NULL, this, "MouseButtonUp", &TransformModeControls::OnMouseUp);
        }

        void TransformModeControls::Update() {
            if(EntitySelected) {
                Entity* entity = ForLease->GameStateManager().CurrentState().GetEntityByID(ActiveEntity);
                if(entity) {
                    Components::Transform* trans = entity->GetComponent<Components::Transform>();
                    Components::Collision* collider = entity->GetComponent<Components::Collision>();
                    ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>()->SetDrawingColor(1, 0, 0, 1);
                    ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>()->DrawRectangle(trans->Position, collider->Width, collider->Height);
                }
            }
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

        void TransformModeControls::OnMouseUp(Event const* e) {
            MouseButtonEvent const* mouse_e = static_cast<MouseButtonEvent const*>(e);

            if(!Active && mouse_e->Clicks == 2) {
                Activate();
            }

            if(Active) {
                Point worldLoc = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>()->ScreenToWorld(mouse_e->ScreenLocation);
                Entity* entity = ForLease->GameStateManager().CurrentState().GetEntityAtPosition(worldLoc);
                if(!entity || !entity->HasComponent(ComponentType::DragWithMouse)) {
                    if(EntitySelected) {
                        Entity* selected = ForLease->GameStateManager().CurrentState().GetEntityByID(ActiveEntity);
                        if(selected) {
                            ForLease->GameStateManager().CurrentState().GetEntityByID(ActiveEntity)->GetComponent<Components::DragWithMouse>()->Active = false;
                        }
                    }
                    EntitySelected = false;
                }
                else {
                    Entity* selected = ForLease->GameStateManager().CurrentState().GetEntityByID(ActiveEntity);
                    if(selected) {
                        ForLease->GameStateManager().CurrentState().GetEntityByID(ActiveEntity)->GetComponent<Components::DragWithMouse>()->Active = false;
                    }
                    EntitySelected = true;
                    ActiveEntity = entity->GetID();
                    entity->GetComponent<Components::DragWithMouse>()->Active = true;
                }
            }
        }
    }
}

/*!
    \file   ComponentTransformModeControls.cpp
    \author Christopher Hudson

    \brief
        Defines a component for switching between the two gameplay modes

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

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
                                                      ModeToggleKey(Keys::LeftAlt), TransformModeSound(""), Active(false), EntitySelected(false), ActiveEntity(0) {}

        TransformModeControls::~TransformModeControls() {
            ForLease->Dispatcher.Detach(this, "KeyUp");
            ForLease->Dispatcher.Detach(this, "MouseButtonUp");
        }

        ComponentType TransformModeControls::GetType() {
            return Type;
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
                    ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>()->DrawRectangle(trans->Position, collider->Width * trans->ScaleX, collider->Height * trans->ScaleY);
                }
            }
        }

        void TransformModeControls::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned int>(Type));
            Serializer transModeCont = root.GetChild("TransformModeControls");
            transModeCont.WriteFloat("SlowMotionSpeed", SlowMotionSpeed);
            transModeCont.WriteFloat("NormalSpeed", NormalSpeed);
            transModeCont.WriteInt("ModeToggleKey", ModeToggleKey);
            transModeCont.WriteString("TransformModeSound", TransformModeSound);
            transModeCont.WriteUint("Type", static_cast<unsigned int>(Type));
            root.Append(transModeCont, "TransformModeControls");
        }

        void TransformModeControls::Deserialize(Serializer& root) {
            Serializer transModeCont = root.GetChild("TransformModeControls");
            transModeCont.ReadFloat("SlowMotionSpeed", SlowMotionSpeed);
            transModeCont.ReadFloat("NormalSpeed", NormalSpeed);
            transModeCont.ReadInt("ModeToggleKey", ModeToggleKey);
            transModeCont.ReadString("TransformModeSound", TransformModeSound);
        }

        void TransformModeControls::Activate() {
            Active = true;

            if(EntitySelected) {
                Entity* entity = ForLease->GameStateManager().CurrentState().GetEntityByID(ActiveEntity);

                if(entity) {
                    if(entity->HasComponent(ComponentType::DragWithMouse)) {
                        entity->GetComponent<Components::DragWithMouse>()->Active = true;
                    }
                    if(entity->HasComponent(ComponentType::ScaleWithKeyboard)) {
                        entity->GetComponent<Components::ScaleWithKeyboard>()->Active = true;
                    }
                }
            }
            SoundEmitter* emitter = Parent.GetComponent<Components::SoundEmitter>();
            if(emitter) {
                emitter->Play(TransformModeSound);
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
                    if(entity->HasComponent(ComponentType::ScaleWithKeyboard)) {
                        entity->GetComponent<Components::ScaleWithKeyboard>()->Active = false;
                    }
                }

                EntitySelected = false;
            }

            SoundEmitter* emitter = Parent.GetComponent<SoundEmitter>();
            if(emitter)
                emitter->Stop();

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
                Entity* entity = ForLease->GameStateManager().CurrentState().GetEntityCollidingAtPoint(worldLoc);

                if(!entity || (!entity->HasComponent(ComponentType::DragWithMouse) && !entity->HasComponent(ComponentType::ScaleWithKeyboard))) {
                    if(EntitySelected) {
                        Entity* selected = ForLease->GameStateManager().CurrentState().GetEntityByID(ActiveEntity);
                        if(selected) {
                            ForLease->GameStateManager().CurrentState().GetEntityByID(ActiveEntity)->GetComponent<Components::DragWithMouse>()->Active = false;
                            ForLease->GameStateManager().CurrentState().GetEntityByID(ActiveEntity)->GetComponent<Components::ScaleWithKeyboard>()->Active = false;
                        }
                    }
                    EntitySelected = false;
                }
                else {
                    Entity* selected = ForLease->GameStateManager().CurrentState().GetEntityByID(ActiveEntity);
                    if(selected) {
                        ForLease->GameStateManager().CurrentState().GetEntityByID(ActiveEntity)->GetComponent<Components::DragWithMouse>()->Active = false;
                        ForLease->GameStateManager().CurrentState().GetEntityByID(ActiveEntity)->GetComponent<Components::ScaleWithKeyboard>()->Active = false;
                    }
                    EntitySelected = true;
                    ActiveEntity = entity->GetID();
                    if(entity->HasComponent(ComponentType::DragWithMouse)) {
                        entity->GetComponent<Components::DragWithMouse>()->Active = true;
                    }
                    if(entity->HasComponent(ComponentType::ScaleWithKeyboard)) {
                        entity->GetComponent<Components::ScaleWithKeyboard>()->Active = true;
                    }
                }
            }
        }
    }
}
